/*
This program will allow you to view it's current dynamic memory usage on a visual bar graph, and also provides a few ways to control it as well, allowing you to experiment and see
how different actions can affect how this app (and your computer) performs, as well as why memory leaks should be avoided by taking the appropriate precations when
developing a program that is actually meant to be useful.

This is just beginning to tap into my full potential as a software developer. While this may look somewhat basic,
it is important to note that this is just scratching the surface of what I am capable of. More advanced and adaptable versions of this and many other kinds of programs
can be created using the skills I have.

DISCLAIMER: This program is a DEMO PROGRAM, one I created for the express purpose of giving others a taste of what I am capable of with my computer programming skills.
I do not intend for it to be useful for any real world purpose, and I advise that you don't expect that from it either.

README: For more info about what this program does, and to learn about it's licencing restrictions, please refer to the README.txt file that is packaged with this one

To run this program on your own device, you will need to download the EXE to your Windows PC, or build it's code using a C++ compiler. This program was created for Windows 10.
While it may work on other operating systems, I have not tested for that and therefore cannot guarentee as such.
*/


#pragma once
#define ALLOCATE 1
#define DEALLOCATE 2
#define LEAK 3
#include "FontData.h" // Include for needed font data
#include <windows.h>
#include <SFML/Graphics.hpp>

extern char* AlloPtr;
struct MemStats { // Struct to store the current memory stats
	MemStats(float Active, float Total, float Available) {
		ActiveMB = Active;
		TotalMB = Total;
		AvailableMB = Available;
	}
	const float GetTotalUsage() {
		float UsagePer = (TotalMB / AvailableMB) * 100;
		return UsagePer;
	}
	const float GetActiveUsage() {
		float UsagePer = (ActiveMB / AvailableMB) * 100;
		return UsagePer;
	}
	const float GetActiveMB() {
		return ActiveMB;
	}
	const float GetTotalMB() {
		return TotalMB;
	}
	const float GetAvailableMB() {
		return AvailableMB;
	}
	void ThrowError(std::string Error) {
		LastError = Error;
	}
	std::string GetLastError() {
		std::string Error = LastError;
		LastError = "";
		return Error;
	}
private:
	float ActiveMB;
	float TotalMB;
	float AvailableMB;
	std::string LastError = "";
};
using namespace sf;
MemStats ManageMemory(int mode, MemStats CurrentStats) { // Allows the user to allocate, deallocate, and leak the program's memory.
	std::string Error = "";
	float NewActiveMB = CurrentStats.GetActiveMB();
	float NewTotalMB = CurrentStats.GetTotalMB();
	if (mode == ALLOCATE) {
		try {
			if (CurrentStats.GetAvailableMB() - 100 > 0) {
				delete[] AlloPtr;
				AlloPtr = new char[(CurrentStats.GetActiveMB() + 100) * (1024 * 1024)];
				NewActiveMB = NewActiveMB + 100;
				NewTotalMB = NewTotalMB + 100;
			}
			else {
				Error = "Memory Allocation Failed";
			}

		}
		catch (const std::bad_alloc) {
			Error = "Memory Allocation Failed";
		}
	}
	else if (mode == DEALLOCATE) {
		if (CurrentStats.GetActiveMB() >= 100) {
			delete[] AlloPtr;
			AlloPtr = new char[static_cast<float>((CurrentStats.GetActiveMB() - 100) * (1024 * 1024))];
			NewActiveMB = NewActiveMB - 100;
			NewTotalMB = NewTotalMB - 100;
		}
		else {
			Error = "Memory Deallocation Failed";
		}
	}
	else if (mode == LEAK) {
		if (CurrentStats.GetActiveMB() != 0) {
			AlloPtr = new char[10];
			NewActiveMB = 0;
		}
		else {
			Error = "Memory Leaking sim Failed";
		}
	}
	MemStats NewStats(NewActiveMB, NewTotalMB, CurrentStats.GetAvailableMB());
	if (Error.compare("") != 0) {
		NewStats.ThrowError(Error);
	}
	return NewStats;
}
MemStats GetMemoryStats(float ActiveUsageMB, float TotalUsageMB) { // Function used to obtain the current available memory, which is stored in the MemStats object.
	float MemAvail = 0;
	MEMORYSTATUSEX MemStat;
	MemStat.dwLength = sizeof(MemStat);
	if (GlobalMemoryStatusEx(&MemStat)) {
		MemAvail = MemStat.ullAvailPhys / static_cast<float>(1024 * 1024);
	}
	MemStats NewMemStats(ActiveUsageMB, TotalUsageMB, MemAvail);
	return NewMemStats;
}

Vector2i DefaultPos() { // Very basic function used to reset the last click position of the mouse to the default after program startup, or after clicking (to prevent unintended repeated controls)
	return Vector2i(0, 0);
}

bool RectContains(Vector2i pos, RectangleShape rect) { // Takes in the currnet mouse position, and a rectange shape, then returns a bool to indicate whether or not that shape contains the mouse position
	FloatRect RectBounds = rect.getGlobalBounds();
	Vector2f Vector = Vector2f(static_cast<float>(pos.x), static_cast<float>(pos.y));
	if (RectBounds.contains(Vector)) {
		return true;
	}
	return false;
}
RectangleShape GraphPercent(RectangleShape CurrentGraphObject, float percent, float ObjectMaxLength) { // Modifies the visual bar graphs in the app to make sure they always correctly resemble their respective values. Caps the graph display at 100% to prevent instances where the graphs show higher values due to memory potential not detectable by this program.
	Vector2f CurrentSize = CurrentGraphObject.getSize();
	float MovementPerPercent = ObjectMaxLength / 100;
	float GraphValue = percent * MovementPerPercent;
	if (GraphValue <= ObjectMaxLength) {
		CurrentGraphObject.setSize(Vector2f(GraphValue, CurrentSize.y));
	}
	else {
		CurrentGraphObject.setSize(Vector2f(ObjectMaxLength, CurrentSize.y));
	}
	return CurrentGraphObject;
}