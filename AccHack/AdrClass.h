#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>
#if (_MSC_VER >= 1400)         // Check MSC version
#pragma warning(push)
#pragma warning(disable: 4996) // Disable deprecation
#endif 
class AdrClass
{
private:
	//DEFINITION DER ATTRIBUTE
	DWORD ClassBaseAdress; //Hauptadresse des Speichers
	DWORD* ClassOffsets; //Offset array um beschreibbare Adresse herrauszufinden
	char ClassValue[]; //eingesetzter Wert

public:
	//DEFINITION DER METHODEN
	//KONSTRUKTOR
	AdrClass(char* Value, DWORD BaseAdress, int OffsetNumber, DWORD* Offsets); //Definiert die Attribute
	//DESTRUKTIOR
	~AdrClass(); //Gibt dynamischen Speicher frei damit kein Memory Leak entsteht

	// GET METHODEN
	DWORD GetClassBaseAdress(); //Gibt BaseAdress zur�ck
	DWORD* GetClassOffsets(); //Gibt Pointer auf Offset Array zur�ck
	char* GetClassValue(); //Gibt Wert der eingef�gt wird als C-Array zur�ck
};


AdrClass::AdrClass(char* Value, DWORD BaseAdress, int OffsetNumber, DWORD* Offsets)
{
	ClassOffsets = new DWORD[OffsetNumber];
	AdrClass::ClassBaseAdress = BaseAdress;
	for (int i = 0; i < OffsetNumber; i++)
	{
		AdrClass::ClassOffsets[i] = Offsets[i];
	}

	strcpy(AdrClass::ClassValue, Value);
}

AdrClass::~AdrClass()
{
	delete[] ClassOffsets;
}

DWORD AdrClass::GetClassBaseAdress()
{
	return AdrClass::ClassBaseAdress;
}
DWORD* AdrClass::GetClassOffsets()
{
	return AdrClass::ClassOffsets;
}
char* AdrClass::GetClassValue()
{
	return AdrClass::ClassValue;
}







