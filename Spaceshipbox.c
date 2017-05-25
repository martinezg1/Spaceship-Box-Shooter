//Maddie Duggan, Chaz Hamel, Glenn Martinez
//04/11/17
//Project: Spaceship Box Shooter
// This code is is part of a timer interrupt example code. 

#include "address_map_nios2.h"
#include "key_codes.h" 	// defines values for KEY0, KEY1, ...
#include <stdio.h>
extern volatile int key_pressed, pattern, shift_dir;
volatile int * PS2_ptr = (int *) PS2_BASE;

void drawpixel(int x_vga, int y_vga, short color) //enables to print pixels on screen
{       volatile short *pixel_address = (volatile short*)(0x08000000 + (y_vga<<10) + (x_vga<<1)); 
       *pixel_address = color;
}

void clearscreen () //clears the screen and colors the background black
{
	int x_vga, y_vga;
	for (x_vga = 0; x_vga <=319; x_vga = x_vga + 1)
	{
		for (y_vga = 0; y_vga <=239; y_vga = y_vga + 1)
		{drawpixel(x_vga, y_vga,  0);}
	}
}
void spaceship() //draws the inital position of the spaceship
{
	int x_vga, y2_vga;
	for (x_vga = 130; x_vga <=180; x_vga = x_vga + 1) 
	{
		for (y2_vga = 200; y2_vga <=239; y2_vga = y2_vga + 1)
		{
			drawpixel(x_vga, y2_vga, 0xFFFF); 
		}
	}
}
void spaceship_move(int x11, int x22, int y11, int y22, short color, int j) // enables the spaceship to move left and right
{
	int x_vga, y2_vga;
for (x_vga = x11-j; x_vga <=x22-j; x_vga = x_vga + 1) 
	{
		for (y2_vga = y11; y2_vga <=y22; y2_vga = y2_vga + 1)
		{
			drawpixel(x_vga, y2_vga, color); 
		}
	}
}

void boxmaker(int y1, int y2, int x1, int x2, short pixel_color)//allows boxes to be printed on the screen
{
		int offset, row, col;
		row=y1;
			for (row; row < y2; row++)
				{
					col = x1;
					for (col; col<x2;++col)
					{
						drawpixel(row,col,pixel_color);
					}
					col = x1;
				}
}
void VGA_text(int x, int y, char * text_ptr)
{
	int offset;
  	volatile char * character_buffer = (char *) FPGA_CHAR_BASE;	// VGA character buffer

	/* assume that the text string fits on one line */
	offset = (y << 7) + x;
	while ( *(text_ptr) )
	{
		*(character_buffer + offset) = *(text_ptr);	// write to the character buffer
		++text_ptr;
		++offset;
	}
}
void spaceship_shoot(int shoot, int k) // draws a red rectangle from the middle of the spaceship. This is the shooting function of the space ship
{	
	int x3_vga, y3_vga, y1, y2, x1, x2,j;
	int color1, color2, color3;
	for (x3_vga = 153 - shoot; x3_vga <= 158 - shoot; x3_vga = x3_vga + 1) 
	{
		for (y3_vga = 180 - k; y3_vga <= 200 - k; y3_vga = y3_vga + 1)
		{
			drawpixel(x3_vga, y3_vga, 0xF800); 
		}			
	}
}

void interval_timer_isr( )
{		
	clearscreen();
	int open = 1;
	while(1)//overall loop
	{
	
	///////Initialization of Variables/////////////
	volatile int * PS2_ptr = (int *) PS2_BASE;
	volatile int * interval_timer_ptr = (int *) TIMER_BASE;	// Altera timer address
	*(interval_timer_ptr) = 0; // Clear the interrupt
	int PS2_data, RVALID;
	char erase[45] = "                                      \0";
	while (open)
	{
		char menu[40] = "Welcome to Spaceship Box Shooter !\0";
		char menu_bot[40] = "Press any key to continue \0";
		VGA_text (25, 29, menu);
		VGA_text (29, 30, menu_bot);
		PS2_data = *(PS2_ptr);	// read the Data register in the PS/2 port
		RVALID = PS2_data & 0x8000;
		if(RVALID)
		{
		
	VGA_text (25, 29, erase);
	VGA_text (27, 30, erase); 
		open = 0;
		}
	}
    int x3_vga, y3_vga;
	int x_vga, y2_vga, y_vga, shot, k;
    int j= 0;
	int i =0;
	static int x1,x2,y1,y2,color1,color2,color3,color4,color5,color6,color7; x1=0;x2=30;y1=0;y2=10;color1 = 1; color2=1;color3=1;
	color4 = 1; color5=1;color6=1;color7=1;
	int game = 1;
	int pixel_color = 220;
	int restart;
	
	VGA_text (25, 29, erase);
	VGA_text (27, 30, erase); 
	clearscreen();
	spaceship(); //print ship in the middle of the screen
	boxmaker(x1,x2,y1,y2,color1);////////////////////////////////
	boxmaker(x1+45,x2+45,y1,y2,color2);/////////////////////////
	boxmaker(x1+90,x2+90,y1,y2,color3);////////////////////////
	boxmaker(x1+135,x2+135,y1,y2,color4);// Draws Enemies///////
	boxmaker(x1+180,x2+180,y1,y2,color5);////////////////////////
	boxmaker(x1+225,x2+225,y1,y2,color6);///////////////////////
	boxmaker(x1+270,x2+270,y1,y2,color7);///////////////////////
	
	while(game)//game loop
	{
		*(interval_timer_ptr+0) =0x0;
		*(interval_timer_ptr+1) =0x4;
		while(*(interval_timer_ptr)!=0x1){}
		i++;

		PS2_data = *(PS2_ptr);	// read the Data register in the PS/2 port
		RVALID = PS2_data & 0x8000;
		int limit = 200;
		if(RVALID) //check to see if there is an input from the PS/2 keyboard
		{	
			switch (PS2_data)
			{
				case 98332: // move left with 'A' key
				if(j==130)//when it reaches the left-hand edge, stop moving
				{j = j -5;}
				j = j + 5; 
				
				clearscreen();
				// draws the green graphic on the spaceship
				spaceship_move(130,180,200,239,0xFFFF,j);
				spaceship_move(154,156,200,209,0x0,j);
				spaceship_move(153,154,200,209,0x07E0,j);
				spaceship_move(152,154,201,209,0x07E0,j);
				spaceship_move(153,154,202,209,0x07E0,j);
				spaceship_move(152,154,203,209,0x07E0,j);
				spaceship_move(153,154,204,209,0x07E0,j);
				spaceship_move(151,154,205,209,0x07E0,j);
				spaceship_move(150,154,206,209,0x07E0,j);
				spaceship_move(151,154,207,209,0x07E0,j);
				spaceship_move(152,154,208,209,0x07E0,j);
				spaceship_move(152,154,209,209,0x07E0,j);
				
				spaceship_move(156,157,200,209,0x07E0,j);
				spaceship_move(156,158,201,209,0x07E0,j);
				spaceship_move(156,157,202,209,0x07E0,j);
				spaceship_move(156,158,203,209,0x07E0,j);
				spaceship_move(156,157,204,209,0x07E0,j);
				spaceship_move(156,159,205,209,0x07E0,j);
				spaceship_move(156,160,206,209,0x07E0,j);
				spaceship_move(156,159,207,209,0x07E0,j);
				spaceship_move(156,158,208,209,0x07E0,j);
				spaceship_move(156,154,209,209,0x07E0,j);
				
				//check to see if the boxes have been shot
				if (color1==1)
				{
					boxmaker(x1,x2,y1,y2,pixel_color);
				}
				if (color2==1)
				{
					boxmaker(x1+45,x2+45,y1,y2,pixel_color);
				}
				if (color3==1)
				{
					boxmaker(x1+90,x2+90,y1,y2,pixel_color);
				}
				if (color4==1)
				{
					boxmaker(x1+135,x2+135,y1,y2,pixel_color);
				}
				if (color5==1)
				{
					boxmaker(x1+180,x2+180,y1,y2,pixel_color);
				}
				if (color6==1)
				{
					boxmaker(x1+225,x2+225,y1,y2,pixel_color);
				}
				if (color7==1)
				{
					boxmaker(x1+270,x2+270,y1,y2,pixel_color);
				}
				break;
				
				case 98339: // move right with D key
				if(j == -140)//when it reaches the right-hand edge, stop moving
				{ j = j+ 5;}
				j = j - 5;
				clearscreen();
				// draws the green graphic on the spaceship
				spaceship_move(130,180,200,239,0xFFFF,j); //CAN MAKE IT LOOKS BETTER
				spaceship_move(154,156,200,209,0x0,j);
				spaceship_move(153,154,200,209,0x07E0,j);
				spaceship_move(152,154,201,209,0x07E0,j);
				spaceship_move(153,154,202,209,0x07E0,j);
				spaceship_move(152,154,203,209,0x07E0,j);
				spaceship_move(153,154,204,209,0x07E0,j);
				spaceship_move(151,154,205,209,0x07E0,j);
				spaceship_move(150,154,206,209,0x07E0,j);
				spaceship_move(151,154,207,209,0x07E0,j);
				spaceship_move(152,154,208,209,0x07E0,j);
				spaceship_move(152,154,209,209,0x07E0,j);
				
				spaceship_move(156,157,200,209,0x07E0,j);	
				spaceship_move(156,158,201,209,0x07E0,j);
				spaceship_move(156,157,202,209,0x07E0,j);
				spaceship_move(156,158,203,209,0x07E0,j);
				spaceship_move(156,157,204,209,0x07E0,j);
				spaceship_move(156,159,205,209,0x07E0,j);
				spaceship_move(156,160,206,209,0x07E0,j);
				spaceship_move(156,159,207,209,0x07E0,j);
				spaceship_move(156,158,208,209,0x07E0,j);
				spaceship_move(156,154,209,209,0x07E0,j);
				// check to see if any enemies have been shot. If so, turn off or stop drawing them
				if (color1==1)
				{
					boxmaker(x1,x2,y1,y2,pixel_color);
				}
				if (color2==1)
				{
					boxmaker(x1+45,x2+45,y1,y2,pixel_color);
				}
				if (color3==1)
				{
					boxmaker(x1+90,x2+90,y1,y2,pixel_color);
				}
					if (color4==1)
				{
					boxmaker(x1+135,x2+135,y1,y2,pixel_color);
				}
				if (color5==1)
				{
					boxmaker(x1+180,x2+180,y1,y2,pixel_color);
				}
				if (color6==1)
				{
					boxmaker(x1+225,x2+225,y1,y2,pixel_color);
				}
				if (color7==1)
				{
					boxmaker(x1+270,x2+270,y1,y2,pixel_color);
				}
				break;
				
				case 98345://shooting case: when space bar is pressed, draw a red rectangle to the top of the screen
				for( k = 0; k <= limit ; k++)
				{		
				spaceship_shoot(j,k);
				// check to see if any enemies have been shot. If so, turn off or stop drawing them
				if (color1==1)
				{
					boxmaker(x1,x2,y1,y2,pixel_color);
				}
				if (color2==1)
				{
					boxmaker(x1+45,x2+45,y1,y2,pixel_color);
				}
				if (color3==1)
				{
					boxmaker(x1+90,x2+90,y1,y2,pixel_color);
				}
					if (color4==1)
				{
					boxmaker(x1+135,x2+135,y1,y2,pixel_color);
				}
				if (color5==1)
				{
					boxmaker(x1+180,x2+180,y1,y2,pixel_color);
				}	
				if (color6==1)
				{
					boxmaker(x1+225,x2+225,y1,y2,pixel_color);
				}
				if (color7==1)
				{
					boxmaker(x1+270,x2+270,y1,y2,pixel_color);
				}
				//////////Hit Detection////////////////////// detects by checking the value of j or the incrementing of j as it moves left and right
					if(color1==1)
					{
					if ((j >= 125)&&(j <= 157))
						{
							color1 =0; //change box 1 to black
						}
					}
					if(color2==1)
					{
					if ((j >= 80)&&(j<=110))
						{
							color2 = 0; //change box 2 to black
						}
					}
					if(color3 == 1)
					{
					if ((j >=40)&&(j<=60))
						{
							color3 = 0;//change box 3 to black
						}
					}
					if(color4 == 1)
					{
					if ((j >=-5)&&(j<=20))
						{
							color4 = 0;//change box 3 to black
						}
					}
					if(color5 == 1)
					{
					if ((j >=-50)&&(j<=-25))
						{
						
						color5 = 0;//change box 5 to black
						}
					}
					if(color6 == 1)
					{
					if ((j >=-95)&&(j<=-70))
						{
						color6 = 0;//change box 6 to black
						}
					}
					if(color7 == 1)
					{
					if ((j >=-140)&&(j<=-115))
						{
						color7 = 0;//change box 7 to black
						}
					}
				////////////////////////////////////////////////
				}
				break;
			}
		}
		if ( i == 100)// check to see if i counted to 100.
		{
			i = 0;
			y1=y1+5;// move boxes down 5 pixels
			y2=y2+5;
			clearscreen();
				if (color1==1)
				{
					boxmaker(x1,x2,y1,y2,pixel_color);
				}
				if (color2==1)
				{
					boxmaker(x1+45,x2+45,y1,y2,pixel_color);
				}
				if (color3==1)
				{
					boxmaker(x1+90,x2+90,y1,y2,pixel_color);
				}
					if (color4==1)
				{
					boxmaker(x1+135,x2+135,y1,y2,pixel_color);
				}
				if (color5==1)
				{
					boxmaker(x1+180,x2+180,y1,y2,pixel_color);
				}
				if (color6==1)
				{
					boxmaker(x1+225,x2+225,y1,y2,pixel_color);
				}
				if (color7==1)
				{
					boxmaker(x1+270,x2+270,y1,y2,pixel_color);
				}
			
			// draws the green graphic on the spaceship
			spaceship_move(130,180,200,239,0xFFFF,j);
			spaceship_move(154,156,200,209,0x0,j);
			spaceship_move(153,154,200,209,0x07E0,j);
			spaceship_move(152,154,201,209,0x07E0,j);
			spaceship_move(153,154,202,209,0x07E0,j);
			spaceship_move(152,154,203,209,0x07E0,j);
			spaceship_move(153,154,204,209,0x07E0,j);
			spaceship_move(151,154,205,209,0x07E0,j);
			spaceship_move(150,154,206,209,0x07E0,j);
			spaceship_move(151,154,207,209,0x07E0,j);
			spaceship_move(152,154,208,209,0x07E0,j);
			spaceship_move(152,154,209,209,0x07E0,j);
			
			spaceship_move(156,157,200,209,0x07E0,j);
			spaceship_move(156,158,201,209,0x07E0,j);
			spaceship_move(156,157,202,209,0x07E0,j);
			spaceship_move(156,158,203,209,0x07E0,j);
			spaceship_move(156,157,204,209,0x07E0,j);
			spaceship_move(156,159,205,209,0x07E0,j);
			spaceship_move(156,160,206,209,0x07E0,j);
			spaceship_move(156,159,207,209,0x07E0,j);
			spaceship_move(156,158,208,209,0x07E0,j);
			spaceship_move(156,154,209,209,0x07E0,j);
			
			//losing state
				if (y2 == 200) //check to see if the the boxes reach the top of the spaceship or reaches when y-axis = 200
				{ 
				// restart the game to initial position
					y1 = 0;
					y2 = 10;
					color1 = 220;
					color2 = 220;
					color3 =220;
					color4 = 220;
					color5 = 220;
					color6 = 220;
					color7 = 220;
					printf("You Lose \n");
					printf("Press r to restart\n");
					char loser[40] = "You Lose !\0";
					char pressr[40] = "Press 'R' to restart\0";
					VGA_text (37, 29, loser);
					VGA_text (32, 30, pressr);
					restart = 1;
			while(restart)
			{
			PS2_data = *(PS2_ptr);	// read the Data register in the PS/2 port
			RVALID = PS2_data & 0x8000;
			if(RVALID)
			{
				if(PS2_data == 98349)
				{
					char rest[40] = "RESTARTING...\0";
					VGA_text (36, 29, rest);
					printf("RESTARTING.... \n");
				clearscreen();
				game = 0;
				restart = 0;
				}
			}	
		}
			}
		}
		// winning state: check to see if all boxes are turned off
		if((color1 == 0) && (color2 == 0) && (color3 == 0)&& (color3 == 0) && (color4 == 0)&& (color5 == 0) && (color6 == 0)&& (color7 ==0))
		{
			// restart the game to initial position
			printf(" YOU ARE A WINNER \n");
			printf("Press r to restart\n");			
			restart = 1;
			char winner[40] = "You Win !\0";
			char pressr[40] = "Press 'R' to restart\0";
			VGA_text (38, 29, winner);
			VGA_text (32, 30, pressr);
			while(restart)
			{
			PS2_data = *(PS2_ptr);	// read the Data register in the PS/2 port
			RVALID = PS2_data & 0x8000;
			if(RVALID)
			{
				if(PS2_data == 98349)
				{
				char rest[40] = "RESTARTING...\0";
				VGA_text (36, 29, rest);
				printf("RESTARTING.... \n");
				clearscreen();
				game = 0;
				restart = 0;
				}
			}	
			}
		}
	}		
	}
 return;
}

