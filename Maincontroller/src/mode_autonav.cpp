/*
 * mode_autonav.cpp
 *
 *  Created on: 2021年8月26日
 *      Author: 25053
 */
#include "maincontroller.h"

static float target_yaw=0.0f;
static float thrust=0.0f;
static bool jump=false;
bool mode_autonav_init(void){
	if(motors->get_armed()){//电机未锁定,禁止切换至该模式
		Buzzer_set_ring_type(BUZZER_ERROR);
		return false;
	}
	if(!pos_control->is_active_z()){
		// initialize position and desired velocity
		pos_control->set_alt_target_to_current_alt();
		pos_control->set_desired_velocity_z(get_vel_z());
	}
	set_manual_throttle(false);//设置为自动油门
	Buzzer_set_ring_type(BUZZER_MODE_SWITCH);
	usb_printf("switch mode autonav!\n");
	return true;
}

void mode_autonav(void){
	AltHoldModeState althold_state;
	float takeoff_climb_rate = 0.0f;
	float ch7=get_channel_7();
	// initialize vertical speeds and acceleration
	pos_control->set_speed_z(-param->pilot_speed_dn.value, param->pilot_speed_up.value);
	pos_control->set_accel_z(param->pilot_accel_z.value);
	pos_control->set_speed_xy(param->poshold_vel_max.value);
	float vel_2d=sqrtf(sq(get_vel_x(),get_vel_y()));
	if(vel_2d<500){
		pos_control->set_accel_xy(param->poshold_accel_max.value);
	}else if(vel_2d<800){
		pos_control->set_accel_xy(param->poshold_accel_max.value/2);
	}else{
		pos_control->set_accel_xy(param->poshold_accel_max.value/3);
	}

	// get pilot desired lean angles
	float target_roll, target_pitch;
	get_pilot_desired_lean_angles(target_roll, target_pitch, param->angle_max.value, attitude->get_althold_lean_angle_max());

	// get pilot's desired yaw rate
	float target_yaw_rate = get_pilot_desired_yaw_rate(get_channel_yaw_angle());

	// get pilot desired climb rate
	float target_climb_rate = get_pilot_desired_climb_rate(get_channel_throttle());
	target_climb_rate = constrain_float(target_climb_rate, -param->pilot_speed_dn.value, param->pilot_speed_up.value);

	// Alt Hold State Machine Determination
	if (!motors->get_armed()) {
		althold_state = AltHold_MotorStopped;
	} else if (takeoff_running() || takeoff_triggered(target_climb_rate) || get_takeoff()) {
		althold_state = AltHold_Takeoff;
	} else if (ap->land_complete) {
		althold_state = AltHold_Landed;
	} else {
		althold_state = AltHold_Flying;
	}

	// Alt Hold State Machine
	switch (althold_state) {

	case AltHold_MotorStopped:
		robot_state=STATE_STOP;
		if(robot_state_desired==STATE_FLYING||robot_state_desired==STATE_TAKEOFF){
			arm_motors();
		}
		motors->set_desired_spool_state(Motors::DESIRED_SHUT_DOWN);
		attitude->input_euler_angle_roll_pitch_euler_rate_yaw(target_roll, target_pitch, target_yaw_rate);
		attitude->set_yaw_target_to_current_heading();
		target_yaw=ahrs_yaw_deg();
		pos_control->set_xy_target(get_pos_x(), get_pos_y());
		pos_control->reset_predicted_accel(get_vel_x(), get_vel_y());
		pos_control->relax_alt_hold_controllers(0.0f);   // forces throttle output to go to zero
		pos_control->update_z_controller(get_pos_z(), get_vel_z());
		break;

	case AltHold_Takeoff:
		robot_state=STATE_TAKEOFF;
		// set motors to full range
		motors->set_desired_spool_state(Motors::DESIRED_THROTTLE_UNLIMITED);

		// initiate take-off
		if (!takeoff_running()) {
			takeoff_start(constrain_float(param->pilot_takeoff_alt.value,0.0f,1000.0f));
			// indicate we are taking off
			set_land_complete(false);
			// clear i terms
			set_throttle_takeoff();
			set_thr_force_decrease(false);//起飞时禁止限制油门
//			jump=true;
		}

		// get take-off adjusted pilot and takeoff climb rates
		get_takeoff_climb_rates(target_climb_rate, takeoff_climb_rate);

		// call attitude controller
		if(ch7>=0.7&&ch7<=1.0){//姿态模式
			target_yaw=ahrs_yaw_deg();
			attitude->input_euler_angle_roll_pitch_euler_rate_yaw(target_roll, target_pitch, target_yaw_rate);
			pos_control->set_xy_target(get_pos_x(), get_pos_y());
			pos_control->reset_predicted_accel(get_vel_x(), get_vel_y());
		}else{//位置模式
			pos_control->set_pilot_desired_acceleration(target_roll, target_pitch, target_yaw, _dt);
			pos_control->calc_desired_velocity(_dt);
			pos_control->update_xy_controller(_dt, get_pos_x(), get_pos_y(), get_vel_x(), get_vel_y());
			target_yaw=ahrs_yaw_deg();
			attitude->input_euler_angle_roll_pitch_euler_rate_yaw(pos_control->get_roll(), pos_control->get_pitch(), target_yaw_rate);
		}
		// call position controller
		if(jump){//起飞时直接跳起30cm
			if(thrust>0.3){//0.3是阈值，只需要比悬停油门大一点, 需要根据不同的机型自行调整
				thrust=0;
				pos_control->get_accel_z_pid().set_integrator(250);//set i, 需要根据不同的机型自行调整
				pos_control->set_alt_target(get_pos_z()+30);//设置目标高度比当前高度高30cm
				jump=false;
			}else{//jump
				thrust+=0.005;
				attitude->set_throttle_out(thrust, true, 10.0f);
				break;
			}
		}
		pos_control->set_alt_target_from_climb_rate_ff(target_climb_rate, _dt, false);
		pos_control->add_takeoff_climb_rate(takeoff_climb_rate, _dt);
		pos_control->update_z_controller(get_pos_z(), get_vel_z());
		break;

	case AltHold_Landed:
		robot_state=STATE_LANDED;
		// set motors to spin-when-armed if throttle below deadzone, otherwise full range (but motors will only spin at min throttle)
		if (target_climb_rate < 0.0f) {
			motors->set_desired_spool_state(Motors::DESIRED_SPIN_WHEN_ARMED);
		} else {
			motors->set_desired_spool_state(Motors::DESIRED_THROTTLE_UNLIMITED);
		}
		if(robot_state_desired==STATE_DRIVE||robot_state_desired==STATE_LANDED){
			disarm_motors();
		}
		attitude->set_yaw_target_to_current_heading();
		target_yaw=ahrs_yaw_deg();
		attitude->input_euler_angle_roll_pitch_euler_rate_yaw(target_roll, target_pitch, target_yaw_rate);
		pos_control->set_xy_target(get_pos_x(), get_pos_y());
		pos_control->reset_predicted_accel(get_vel_x(), get_vel_y());
		pos_control->relax_alt_hold_controllers(0.0f);   // forces throttle output to go to zero
		pos_control->update_z_controller(get_pos_z(), get_vel_z());
		break;

	case AltHold_Flying:
		robot_state=STATE_FLYING;
		motors->set_desired_spool_state(Motors::DESIRED_THROTTLE_UNLIMITED);

		// call attitude controller
		if(ch7>=0.7&&ch7<=1.0){//姿态模式
			target_yaw+=target_yaw_rate*_dt;
			get_air_resistance_lean_angles(target_roll, target_pitch, DEFAULT_ANGLE_MAX, 1.0f);
			attitude->input_euler_angle_roll_pitch_yaw(target_roll, target_pitch, target_yaw, true);
			pos_control->set_xy_target(get_pos_x(), get_pos_y());
			pos_control->reset_predicted_accel(get_vel_x(), get_vel_y());
		}else if(ch7>0.3&&ch7<0.7){//位置模式
			target_yaw+=target_yaw_rate*_dt;
			pos_control->set_pilot_desired_acceleration(target_roll, target_pitch, target_yaw, _dt);
			pos_control->calc_desired_velocity(_dt);
			pos_control->update_xy_controller(_dt, get_pos_x(), get_pos_y(), get_vel_x(), get_vel_y());
			attitude->input_euler_angle_roll_pitch_yaw(pos_control->get_roll(), pos_control->get_pitch(), target_yaw, true);
		}else{//自主模式
			pos_control->set_speed_xy(param->mission_vel_max.value);
			pos_control->set_accel_xy(param->mission_accel_max.value);
			pos_control->set_xy_target(get_mav_x_target(), get_mav_y_target());
			pos_control->update_xy_controller(_dt, get_pos_x(), get_pos_y(), get_vel_x(), get_vel_y());
			attitude->input_euler_angle_roll_pitch_yaw(pos_control->get_roll(), pos_control->get_pitch(), get_mav_yaw_target(), true);
		}

		if(robot_state_desired==STATE_DRIVE||robot_state_desired==STATE_LANDED){//自动降落
			target_climb_rate=-constrain_float(param->auto_land_speed.value, 0.0f, param->pilot_speed_dn.value);//设置降落速度cm/s
		}

		// adjust climb rate using rangefinder
		target_climb_rate = get_surface_tracking_climb_rate(target_climb_rate, pos_control->get_alt_target(), _dt);

		// call position controller
		pos_control->set_alt_target_from_climb_rate_ff(target_climb_rate, _dt, false);

		pos_control->update_z_controller(get_pos_z(), get_vel_z());
		break;
	default:
		break;
	}
	attitude->rate_controller_run();
	motors->output();
}
