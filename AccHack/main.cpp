
#include "AdrClass.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime> // needed for our timer clock
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
void WriteToMemory(HANDLE hProcHandle);
DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress);


LPCSTR  LGameWindow = "AssaultCube"; //<- MAKE SURE it matches the window name
std::string GameStatus;
//FUNCTION PROTOTYPES
bool IsGameAvail;
bool UpdateOnNextRun; //used to update the display menu only when something changed

//-------AMMO VARS--------
//number we are going to overwrite the current ammo with in bytes
bool AmmoStatus; // used to DEFine wether ammo is on or not
DWORD AmmoOffsets[] = { 0x378, 0x14, 0x0 }; //3 LEVEL pointer
AdrClass* Munition = new AdrClass("\x39\x5", 0x004DF73C, 3, AmmoOffsets);


DWORD RFOffsets[] = { 0x378, 0x18, 0x0 }; //3 LEVEL pointer
AdrClass* RapidFire = new AdrClass("\x00", 0x004DF73C, 3, RFOffsets);
//-------HEALTH VARS--------
bool HealthStatus; // used to DEFine wether ammo is on or not
DWORD HealthOffsets[] = { 0xF4 }; // 1 level pointer
AdrClass* Leben = new AdrClass("\x39\x5", 0x004DF73C, 1, HealthOffsets);

//-------GRANATES VARS--------
bool GranateStatus;
DWORD GranateOffsets[] = { 0x154 }; // 1 level pointer
AdrClass* Granate = new AdrClass("\x39\x5", 0x004DF73C, 1, GranateOffsets);

//-------Speed VARS--------
bool SpeedStatus;
DWORD SpeedOffsets[] = { 0x7C }; // 1 level pointer
AdrClass* Geschwindigkeit = new AdrClass("\x05", 0x004DF73C, 1, SpeedOffsets);
AdrClass* DefGeschwindigkeit = new AdrClass("\x01", 0x004DF73C, 1, SpeedOffsets);
int main()
{


	//Declare our handles as NULL to avoid crashes when closing if they were unused e.g. player starts trainer and closes it before doing any cheats
	HWND hGameWindow = NULL;
	int timeSinceLastUpdate = clock(); //forces status update every x seconds
	int GameAvailTMR = clock();
	int OnePressTMR;//used to limit keys input to only one per x ms
	DWORD dwProcId = NULL;
	HANDLE hProcHandle = NULL;
	UpdateOnNextRun = true;
	std::string sAmmoStatus;
	std::string sHealthStatus;
	std::string sGranateStatus;
	std::string sSpeedStatus;
	sAmmoStatus = "Aus";
	sHealthStatus = "Aus";
	sGranateStatus = "Aus";
	sSpeedStatus = "Aus";
	OnePressTMR = clock();

	while (!GetAsyncKeyState(VK_F12)) //Key is not = 'INSERT'
	{
		//Does a series of checks every x ms and 
		//checks that the game is available and capable of being
		//written to, if thats the case we write declare it available
		//otherwise we report where it went wrong
		//e.g. if game is closed we make things unavailable, or if its opened 
		//we make options available again

		if (clock() - GameAvailTMR > 100)
		{
			GameAvailTMR = clock();
			//Declare game unavailable by default
			//if it is available then it will change immediately
			IsGameAvail = false;
			//Check a valid window is available
			// Get Window Handle 
			hGameWindow = FindWindow(NULL, LGameWindow);
			if (hGameWindow)
			{
				GetWindowThreadProcessId(hGameWindow, &dwProcId);
				//If it is a valid id we continue to try and open the process
				if (dwProcId != 0)
				{
					// Get Process Handle 
					hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcId);
					if (hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL)
					{
						GameStatus = "Der Przess konnte nicht geöffnet werden";
					}
					else
					{
						GameStatus = "Hack bereit";
						IsGameAvail = true;
					}
				}
				else GameStatus = "Prozess Id konnte nicht ermittelt werden";
			}
			else GameStatus = "AssaultCube wurde nicht gefunden";

			//if UpdateNextRun is called or a number of seconds without updates have gone by an auto update is done 
			//to make sure game is available etc.
			if (UpdateOnNextRun || clock() - timeSinceLastUpdate > 5000)
			{
				system("cls");
				std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
				std::cout << "*                                                  *" << std::endl;
				std::cout << "*      AssaultCube Hack - 1.0 - AlexBlogs.net      *" << std::endl;
				std::cout << "*                                                  *" << std::endl;
				std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl << std::endl;
				std::cout << "Status des Spiels:" << GameStatus << std::endl << std::endl;
				std::cout << "[F2] Unendlich Mutinition || " << sAmmoStatus << " " << std::endl << std::endl;
				std::cout << "[F3] Godmode ||" << sHealthStatus << std::endl << std::endl;
				std::cout << "[F4] Unendlich Granaten ||" << sGranateStatus << std::endl << std::endl;
				std::cout << "[R] Speedhack ||" << sSpeedStatus << std::endl << std::endl;
				std::cout << "[F12] Exit" << std::endl;
				UpdateOnNextRun = false;
				timeSinceLastUpdate = clock();
			}

			if (IsGameAvail)
			{
				WriteToMemory(hProcHandle);
			}
		}

		//Stops Keys from being spammed e.g. only allow them to be pressed every x milliseconds
		if (clock() - OnePressTMR > 400)
		{
			if (IsGameAvail)
			{
				//DETECTS WHICH KEYS HAVE BEEN PRESSED IN order to turn cheats on and off
				if (GetAsyncKeyState(VK_F2))
				{
					OnePressTMR = clock();
					//Reverts the ammo status e.g. from true to false and vice versa
					AmmoStatus = !AmmoStatus;
					UpdateOnNextRun = true;
					//changes the text to update on next display
					if (AmmoStatus)sAmmoStatus = "AN";
					else sAmmoStatus = "AUS";
				}
				else if (GetAsyncKeyState(VK_F3))
				{
					OnePressTMR = clock();
					HealthStatus = !HealthStatus;
					UpdateOnNextRun = true;
					//changes the text to update on next display
					if (HealthStatus)sHealthStatus = "AN";
					else sHealthStatus = "AUS";
				}
				else if (GetAsyncKeyState(VK_F4))
				{
					OnePressTMR = clock();
					GranateStatus = !GranateStatus;
					UpdateOnNextRun = true;
					//changes the text to update on next display
					if (GranateStatus)sGranateStatus = "AN";
					else sGranateStatus = "AUS";
				}
				else if (GetAsyncKeyState('R'))
				{
					OnePressTMR = clock();
					SpeedStatus = !SpeedStatus;
					UpdateOnNextRun = true;
					//changes the text to update on next display
					if (SpeedStatus)sSpeedStatus = "AN";
					else
					{
						sSpeedStatus = "AUS";
						DWORD GranateAddressToWrite = FindDmaAddy(1, hProcHandle, DefGeschwindigkeit->GetClassOffsets(), DefGeschwindigkeit->GetClassBaseAdress());
						WriteProcessMemory(hProcHandle, (BYTE*)GranateAddressToWrite, DefGeschwindigkeit->GetClassValue(), sizeof(DefGeschwindigkeit->GetClassValue()) - 2, NULL);

					}
				}
			}
		}
	}
	//Close any handles once the program is over
	CloseHandle(hProcHandle);
	CloseHandle(hGameWindow);

	//Returns that action was completed successfuly
	return ERROR_SUCCESS;
}


//Handles Dynamic memory allocation
//Receives how high the pointer level is e.g. 4 levels and from that calculates the initial address
//the offset values and the memory addresses for those offsets
DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{

	//DECLARE BASE ADDRESS
	DWORD pointer = BaseAddress;             // Declare a pointer of DWORD
	//USED TO output the contents in the pointer
	DWORD pTemp;

	DWORD pointerAddr;
	for (int i = 0; i < PointerLevel; i++)
	{
		if (i == 0)
		{
			ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, 4, NULL);
		}
		//add first offset to that address
		pointerAddr = pTemp + Offsets[i];   // Set p1 to content of p + offset

		//Read memory one more time and exit the loop
		ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, 4, NULL);

	}
	return pointerAddr;
}

void WriteToMemory(HANDLE hProcHandle)
{
	if (AmmoStatus)
	{
		//std::cout << sizeof(Munition->GetClassValue()) << std::endl;

		DWORD AmmoAddressToWrite = FindDmaAddy(3, hProcHandle, Munition->GetClassOffsets(), Munition->GetClassBaseAdress());
		WriteProcessMemory(hProcHandle, (BYTE*)AmmoAddressToWrite, Munition->GetClassValue(), sizeof(Munition->GetClassValue()) - 1, NULL);
		DWORD RF = FindDmaAddy(3, hProcHandle, RapidFire->GetClassOffsets(), RapidFire->GetClassBaseAdress());
		WriteProcessMemory(hProcHandle, (BYTE*)RF, RapidFire->GetClassValue(), 1, NULL);

	}

	if (HealthStatus)
	{
		//because health address is only one pointer in we send only to FindDmaAddy
		DWORD HealthAddressToWrite = FindDmaAddy(1, hProcHandle, Leben->GetClassOffsets(), Leben->GetClassBaseAdress());
		WriteProcessMemory(hProcHandle, (BYTE*)HealthAddressToWrite, Leben->GetClassValue(), sizeof(Leben->GetClassValue()) - 1, NULL);
	}
	if (GranateStatus)
	{
		//because health address is only one pointer in we send only to FindDmaAddy
		DWORD GranateAddressToWrite = FindDmaAddy(1, hProcHandle, Granate->GetClassOffsets(), Granate->GetClassBaseAdress());
		WriteProcessMemory(hProcHandle, (BYTE*)GranateAddressToWrite, Granate->GetClassValue(), sizeof(Granate->GetClassValue()) - 1, NULL);
	}
	if (SpeedStatus)
	{
		//because health address is only one pointer in we send only to FindDmaAddy
		DWORD GranateAddressToWrite = FindDmaAddy(1, hProcHandle, Geschwindigkeit->GetClassOffsets(), Geschwindigkeit->GetClassBaseAdress());
		WriteProcessMemory(hProcHandle, (BYTE*)GranateAddressToWrite, Geschwindigkeit->GetClassValue(), sizeof(Geschwindigkeit->GetClassValue()) - 2, NULL);
	}
}
