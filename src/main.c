#include <lpc21xx.h>
#include <string.h>
#include "main_defines.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "keypad.h"
#include "interrupt.h"
#include "fingerprint.h"
#include "uart.h"
#include "rtc.h"
#include "i2c.h"
#include "motor.h"
#include "database.h"
//#include "log.h"
#include "delay.h"
#include "types.h"

s32 admin_pass=0,new_password=0,re_new_password=0;
s32 id=0,password=0,repassword=0;
USER user;
u8 userindex;
u32 finger_result;
extern volatile u32 flag;
int main()
{
	InitLCD();
	RTC_Init(); 
	Init_KPM();
	Uart_Init();
	//InitUART0();
	init_i2c();
	enable_eint0();
	__enable_irq();
	Motor_Init();
SaveAdminPassword(DEFAULT_ADMIN_PASSWORD);

	CmdLCD(CLEAR_LCD);
	StrLCD("====================");
	U0_Str("====================\r\n");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("*****WELCOME TO*****");
	U0_Str("*****WELCOME TO*****\r\n");
	CmdLCD(GOTO_LINE3_POS0);
	StrLCD("***SECUREGATE PRO***");
	U0_Str("***SECUREGATE PRO***\r\n");
	CmdLCD(GOTO_LINE4_POS0);
	StrLCD("====================");
	U0_Str("====================\r\n");
	delay_s(1);
	while(1)
	{
		if(flag==1)
		{
			CmdLCD(CLEAR_LCD);
			StrLCD("  MENU OPTIONS  ");
			U0_Str("MENU OPTIONS \r\n");
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD("password:");
			admin_pass=enter_menu_password();
			if(VerifyAdminPassword(admin_pass))
			{
				CmdLCD(CLEAR_LCD);
				StrLCD("ADMIN PASSWORD MATCHED");
				U0_Str("ADMIN PASSWORD MATCHED\r\n");
				menu:CmdLCD(CLEAR_LCD);
				StrLCD("1.PASS     2.FINGER");
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("3.ADD_USER 4.DEL_USER");
				CmdLCD(GOTO_LINE3_POS0);
				StrLCD("5.CLR_DB   6.AD_PASS");
				CmdLCD(GOTO_LINE4_POS0);
				StrLCD("7.EXIT");
				switch(keyscan())
				{				 
				 case 1: CmdLCD(CLEAR_LCD);
											 StrLCD("ENTER USER ID:");
											 CmdLCD(GOTO_LINE2_POS0);
											 id=enter_user_id_menu();
											 if(id== -2)
											 {
											  goto menu;
											  }
											userindex = SearchUser(id, &user);
											if(userindex != 0xFF)
											 {
												 CmdLCD(CLEAR_LCD);
											   StrLCD("ENTER USER ID:");
												 CmdLCD(GOTO_LINE2_POS0);
												 StrLCD("ID FOUND");
												 U0_Str("ID FOUND\r\n");
												 delay_s(1);
												 CmdLCD(CLEAR_LCD);
											   StrLCD("ENTER CURR PASS:");
												 CmdLCD(GOTO_LINE2_POS0);
												 password=enter_menu_password();
												 if(password== -2)
											 {
											  goto menu;
											  }
												 if(password == user.password)
													 {
														 CmdLCD(GOTO_LINE2_POS0);
														 StrLCD("PASSWORD:MATCHED");
														 U0_Str("PASSWORD MATCHED\r\n");
														 delay_s(1);
														 CmdLCD(CLEAR_LCD);
														 StrLCD("ENTER NEW PASS:");
														 CmdLCD(GOTO_LINE2_POS0);
												     new_password=enter_menu_password();
													 if(new_password== -2)
											 {
											  goto menu;
											  }
														 CmdLCD(CLEAR_LCD);
														 StrLCD("RE ENTER NEW PASS:");
														 CmdLCD(GOTO_LINE2_POS0);
												     re_new_password=enter_menu_password();
													 if(re_new_password== -2)
											 		{
											  			goto menu;
											  		}
														 CmdLCD(GOTO_LINE2_POS0);
														 StrLCD("                    ");
														 CmdLCD(GOTO_LINE2_POS0);
														 if(new_password==re_new_password)
														 {
															 UpdatePassword(id, new_password);
															 CmdLCD(CLEAR_LCD);
															 StrLCD("UPDATED SUCCESSFULLY");
															 U0_Str("UPDATED SUCCESSFULLY\r\n");
															 delay_s(1);
															 goto menu;
														 }
														 else
														 {
															 CmdLCD(CLEAR_LCD);
															 StrLCD("NOT UPDATED");
															 U0_Str("NOT UPDATED\r\n");
															 delay_s(1);
															 goto menu;
														 }
													 }
													 else
													 {
														 CmdLCD(CLEAR_LCD);
														 StrLCD("ENTER CURR PASS:");
												     CmdLCD(GOTO_LINE2_POS0);
														 StrLCD("WRONG PASSWORD");
														 U0_Str("WRONG PASSWORD\r\n");
														 delay_s(1);
														 goto menu;
													 }
												 }
											  else
													 {
														 CmdLCD(CLEAR_LCD);
														 StrLCD("ENTER USER ID:");
												     CmdLCD(GOTO_LINE2_POS0);
														 StrLCD("ID NOT FOUND");
														 U0_Str("ID NOT FOUND\r\n");
														 delay_s(1);
														 goto menu;
													 }
				case 2:
    CmdLCD(CLEAR_LCD);
    StrLCD("ENTER USER ID:");
    CmdLCD(GOTO_LINE2_POS0);

    id = enter_user_id_menu();
		if(id== -2)
       {
		goto menu;
        }
    /* Search user in EEPROM */
    userindex = SearchUser(id, &user);

    if(userindex == 0xFF)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("ID NOT FOUND");
		U0_Str("ID NOT FOUND\r\n");
        delay_s(1);
        goto menu;
    }
	else
	{
        CmdLCD(CLEAR_LCD);
        StrLCD("ID FOUND");
		U0_Str("ID FOUND\r\n");
		delay_s(2);
    /* ID found - ask password */
    CmdLCD(CLEAR_LCD);
    StrLCD("ENTER PASSWORD");
    CmdLCD(GOTO_LINE2_POS0);

    password = enter_menu_password();
	  if(password== -2)
		{
		goto menu;
		 }
    /* Compare password */
    if(password != user.password)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("WRONG PASSWORD");
		U0_Str("WRONG PASSWORD\r\n");
        delay_s(1);

        CmdLCD(CLEAR_LCD);
        StrLCD("ACCESS DENIED");
		U0_Str("ACCESS DENIED\r\n");
        delay_s(2);

        goto menu;
    }
	 else
	 {
    /* ID + Password matched */
    CmdLCD(CLEAR_LCD);
    StrLCD("PASSWORD MATCHED");
	U0_Str("PASSWORD MATCHED\r\n");
    delay_s(1);

    /* Delete old fingerprint */
    CmdLCD(CLEAR_LCD);
    StrLCD("DELETEING OLD FINGER");
	U0_Str("DELETEING OLD FINGER\r\n");
    delay_s(1);

    if(delete_fp(user.finger_id) != 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("DELETEING FAILED");
		U0_Str("DELETEING FAILEDr\n");
        delay_s(2);
        goto menu;
    }

    /* Enroll new fingerprint */
    CmdLCD(CLEAR_LCD);
    StrLCD("PLACE NEW FINGER");
    delay_s(1);

    if(enroll(user.finger_id) == 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("FINGER UPDATED");
		U0_Str("FINGER UPDATED\r\n");
        delay_s(2);
    }
    else
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("ENROLL FAILED");
		U0_Str("ENROLL FAILED\r\n");
        delay_s(2);
    }
	}
	 }
    goto menu;
				case 3:

												CmdLCD(CLEAR_LCD);
												StrLCD("ENTER NEW USER ID:");
												CmdLCD(GOTO_LINE2_POS0);

												id = enter_user_id_menu();
												 if(id== -2)
												 {
												 goto menu;
												 }
												/* Check whether ID already exists */
												if(SearchUser(id, &user) != 0xFF)
												{
														CmdLCD(CLEAR_LCD);
														StrLCD("ID ALREADY EXIST");
														U0_Str("ID ALREADY EXIST\r\n");
														delay_s(1);
														goto menu;
												}

												/* Check whether fingerprint memory has space */
												user.finger_id = GetFreeFingerID();

												if(user.finger_id == 0xFF)
												{
														CmdLCD(CLEAR_LCD);
														StrLCD("NO FINGER SPACE");
														U0_Str("NO FINGER SPACE\r\n");
														delay_s(1);
														goto menu;
												}

												/* Get user password */
												CmdLCD(CLEAR_LCD);
												StrLCD("ENTER PASSWORD:");
												CmdLCD(GOTO_LINE2_POS0);

												password = enter_menu_password();
												 if(password== -2)
												 {
												 goto menu;
												 }
                       							 CmdLCD(CLEAR_LCD);
												StrLCD("RE ENTER PASSWORD:");
												CmdLCD(GOTO_LINE2_POS0);
												repassword = enter_menu_password();
												 if(repassword== -2)
												 {
												 goto menu;
												 }
												if(password==repassword)
												{

												/* Enroll fingerprint*/ 

												if(enroll(user.finger_id) == 0)
												{
													user.user_id = id;
												user.password = password;
														/* Fingerprint enrollment successful */

														if(AddUser(&user))
														{
																CmdLCD(CLEAR_LCD);
																StrLCD("USER ADDED");
																U0_Str("USER ADDED SUCCESSFULLY\r\n");
																CmdLCD(GOTO_LINE2_POS0);
																StrLCD("SUCCESSFULLY");
																delay_s(2);
														}
														else
														{
																CmdLCD(CLEAR_LCD);
																StrLCD("EEPROM SAVE");
																U0_Str("EEPROM SAVE FAILED\r\n");
																CmdLCD(GOTO_LINE2_POS0);
																StrLCD("FAILED");
																delay_s(2);
														}
												}
												else
												{
														CmdLCD(CLEAR_LCD);
														StrLCD("FINGER ENROLL");
														U0_Str("FINGER ENROLL FAILED\r\n");
														CmdLCD(GOTO_LINE2_POS0);
														StrLCD("FAILED");
														delay_s(2);
												}
											}
												else
													{
														CmdLCD(CLEAR_LCD);
														StrLCD("PASSWORD ENROLL");
														U0_Str("PASSWORD ENROLL FAILED\r\n");
														CmdLCD(GOTO_LINE2_POS0);
														StrLCD("FAILED");
														delay_s(2);
												}
											

												goto menu;
					 case 4:		CmdLCD(CLEAR_LCD);
													StrLCD("ENTER USER ID:");
													CmdLCD(GOTO_LINE2_POS0);

													id = enter_user_id_menu();
													   if(id== -2)
												 {
												 goto menu;
												 }
													/* Search user in EEPROM */
													if(SearchUser(id, &user) == 0xFF)
													{
															CmdLCD(CLEAR_LCD);
															StrLCD("USER NOT FOUND");
															U0_Str("USER NOT FOUND\r\n");
															delay_s(1);
															goto menu;
													}

													/* Delete fingerprint from R305 first */
													CmdLCD(CLEAR_LCD);
													StrLCD("DELETEING FINGER");
													U0_Str("DELETEING FINGER\r\n");
													delay_s(1);

													if(delete_fp(user.finger_id) == 0)
													{
															/* Fingerprint deleted successfully */

															if(DeleteUser(id))
															{
																	CmdLCD(CLEAR_LCD);
																	StrLCD("USER DELETED");
																	U0_Str("USER DELETED\r\n");
																	delay_s(2);
															}
															else
															{
																	CmdLCD(CLEAR_LCD);
																	StrLCD("EEPROM DELETE");
																	U0_Str("EEPROM DELETE FAILED\r\n");
																	CmdLCD(GOTO_LINE2_POS0);
																	StrLCD("FAILED");
																	delay_s(2);
															}
													}
													else
													{
															CmdLCD(CLEAR_LCD);
															StrLCD("FINGER DELETE");
															U0_Str("FINGER DELETE FAILED\r\n");
															CmdLCD(GOTO_LINE2_POS0);
															StrLCD("FAILED");
															delay_s(2);
													}

													goto menu;
					dbclear:case 5: 	CmdLCD(CLEAR_LCD);
									StrLCD("CLEAR DATABASE?");
									CmdLCD(GOTO_LINE2_POS0);
									StrLCD("1.YES  2.NO");

									switch(keyscan())
									{
											case 1:

													CmdLCD(CLEAR_LCD);
													StrLCD("CLEARING...");
													U0_Str("CLEARING...\r\n");
													delay_s(1);

													/* Clear EEPROM user records
														 and reset admin password */
													ClearDatabase();

													/* Delete all fingerprints from R305 */
													if(deleteall_fp() == 0)
													{
															CmdLCD(CLEAR_LCD);
															StrLCD("DATABASE CLEARED");
															U0_Str("DATABASE CLEARED\r\n");
															CmdLCD(GOTO_LINE2_POS0);
															StrLCD("ADMIN PASS:1234");
															U0_Str("ADMIN PASS:1234\r\n");
															delay_s(2);
													}
													else
													{
															CmdLCD(CLEAR_LCD);
															StrLCD("FINGER CLEAR");
															U0_Str("FINGER CLEAR FAILED\r\n");
															CmdLCD(GOTO_LINE2_POS0);
															StrLCD("FAILED");
															delay_s(2);
													}

													goto menu;

											case 2:
													goto menu;

											default:
													CmdLCD(CLEAR_LCD);
													StrLCD("INVALID KEY");
													delay_s(1);
													goto dbclear;
									}
				 case 6:	CmdLCD(CLEAR_LCD);
												StrLCD("ENTER CURR PASS:");
												CmdLCD(GOTO_LINE2_POS0);

												password = enter_menu_password();
												 if(password== -2)
												 {
												 goto menu;
												 }
												if(VerifyAdminPassword(password))
												{
														CmdLCD(CLEAR_LCD);
														StrLCD("ENTER NEW PASS:");
														CmdLCD(GOTO_LINE2_POS0);

														new_password = enter_menu_password();
														 if(new_password== -2)
												 {
												 goto menu;
												 }
														CmdLCD(CLEAR_LCD);
														StrLCD("RE-ENTER NEW:");
														CmdLCD(GOTO_LINE2_POS0);

														re_new_password = enter_menu_password();
														  if(re_new_password== -2)
												 {
												 goto menu;
												 }
														/* Compare before saving */
														if(new_password == re_new_password)
														{
																SaveAdminPassword(new_password);

																CmdLCD(CLEAR_LCD);
																StrLCD("PASSWORD UPDATED");
																U0_Str("PASSWORD UPDATED\r\n");
																delay_s(2);

																goto menu;
														}
														else
														{
																/* DO NOT save to EEPROM */
																CmdLCD(CLEAR_LCD);
																StrLCD("PASSWORD");
																U0_Str("PASSWORD NOT MATCHED\r\n");
																CmdLCD(GOTO_LINE2_POS0);
																StrLCD("NOT MATCHED");
																delay_s(2);

																goto menu;
														}
												}
												else
												{
														CmdLCD(CLEAR_LCD);
														StrLCD("WRONG PASSWORD");
														U0_Str("WRONG PASSWORD\r\n");
														delay_s(2);

														goto menu;
												}
																						 
							  case 7: flag=0;
											  break;
								default:CmdLCD(CLEAR_LCD); 
									      StrLCD("INVALID KEY");
												delay_ms(500);
												goto menu;
												 }
											 }
			else
			{
				CmdLCD(CLEAR_LCD);
				StrLCD("WRONG PASSWORD");
				U0_Str("WRONG PASSWORD\r\n");
				delay_s(5);
				flag = 0;
			}
			}		
				
		start:CmdLCD(CLEAR_LCD);
		StrLCD("USER ID:");
		id=enter_user_id();
		if(id == -1)
			{
				CmdLCD(CLEAR_LCD);
				continue;       // Go back to while(1), where flag==1 will open the admin menu
			}
		if(id == -2)
		{
		goto start;
		}
		userindex = SearchUser(id, &user);

if(userindex != 0xFF)
{
    /* USER ID MATCHED */

    CmdLCD(CLEAR_LCD);
    StrLCD("USER ID:MATCHED");
    U0_Str("USER ID:MATCHED\r\n");
	delay_s(1);
	CmdLCD(CLEAR_LCD);
    StrLCD("USER ID:");
	U0_Str("USER ID:");
	U0_TxU32(id);
	U0_Str("\r\n");
	CmdLCD(0x88);
	U32LCD(id);
    /* Ask Password */
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("PASSWORD:");

    password = enter_password();

    if(password == -1)
    {
        CmdLCD(CLEAR_LCD);
        continue;
    }
	  if(password == -2)
    {
        goto start;
    }
    /* Check Password */

    if(password == user.password)
    {
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("                    ");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("PASSWORD:MATCHED");
        U0_Str("PASSWORD:MATCHED\r\n");
		delay_s(1);

        /* Ask Fingerprint */
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("                    ");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("PLACE FINGER");

        delay_s(1);

        /* Search fingerprint in R305 */

        finger_result = search_fp();

        /* Compare identified fingerprint with user's finger ID */

        if(finger_result == user.finger_id)
        {
            CmdLCD(CLEAR_LCD);
            StrLCD("FINGER MATCHED");
			U0_Str("FINGER:MATCHED\r\n");
            delay_s(1);
            CmdLCD(CLEAR_LCD);
            StrLCD("ACCESS GRANTED");
			U0_Str("ACCESS GRANTED\r\n");
					  CmdLCD(GOTO_LINE2_POS0);
						StrLCD("DOOR OPENING");
						U0_Str("DOOR OPENING\r\n");
					  Door_Open();
					  delay_s(1);
					  CmdLCD(CLEAR_LCD);
					  StrLCD("DOOR CLOSING");
					  U0_Str("DOOR CLOSING\r\n");
					  Door_Close();
					
        }
       else
        {
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("                    ");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("FINGER NOT MATCH");
			U0_Str("FINGER NOT MATCH\r\n");
            delay_s(1);

            CmdLCD(CLEAR_LCD);
            StrLCD("ACCESS DENIED");
			U0_Str("ACCESS DENIED\r\n");
            delay_s(2);
        }
    }
    else
    {
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("                    ");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("PASSWORD WRONG");
		U0_Str("PASSWORD WRONG\r\n");
        delay_s(1);

        CmdLCD(CLEAR_LCD);
        StrLCD("ACCESS DENIED");
		U0_Str("ACCESS DENIED\r\n");
        delay_s(2);
    }
}
else
{
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("                    ");
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("USER ID NOT MATCH");
	U0_Str("USER ID NOT MATCH\r\n");
    delay_s(1);

    CmdLCD(CLEAR_LCD);
    StrLCD("ACCESS DENIED");
	U0_Str("ACCESS DENIED\r\n");
    delay_s(2);
}
}
}
