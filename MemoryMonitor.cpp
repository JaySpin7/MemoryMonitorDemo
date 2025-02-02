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

#include "MemoryMonitor.h"
char* AlloPtr = new char[10];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Font Font;
    if (Font.openFromMemory(FontData, FontDataLen)) { // if the font data loads successfully, the first thing the program does is initialize all the assets that are needed for it to work as intended.
        using namespace sf;
        bool Activated = false;
        std::string Error = "";
        MemStats CurrentMemoryInfo(0, 0, 0);
        Vector2i ClickPos = DefaultPos();
        RenderWindow Window(VideoMode({410, 300}), "Memory Monitor Demo", Style::Titlebar | Style::Close);
        Text TextInitialTitle(Font, "Program Info + Disclaimer:", 30);
        TextInitialTitle.setPosition(Vector2f(20, 3));
        TextInitialTitle.setFillColor(Color::Black);
        Text TextInitialInfo(Font, "PROGRAM INFO:\nThis program will allow you to monitor it's current dynamic memory usage,\nas well as control it, with the assets as follows.\nButtons:\n-Allocate - Dymamically allocates 100MB\n-Deallocate - Dynamically deallocates 100MB\n-Leak - Leaks all of the current dynamically allocated memory, causing the\nprogram to lose control of it. This can only be undone by restarting the app.\nBar Graphs:\n-Active - Total dynamically allocated memory that this program has\ncontrol over (percentage of total available physical memory)\n-Total - Total memory that is currently dynamically allocated by this program,\nincluding both in and out of\nit's control (Also a percentage of total available physical memory)\nDISCLAIMER:\nThis is a demo program, and it is not intended to serve any real world purpose.\nPlease do not expect it to be useful for anything besides seeing my\nskill as a computer programmer! See the README.txt for more details and\nlicensing restrictions.", 10);
        TextInitialInfo.setPosition(Vector2f(30, 43));
        TextInitialInfo.setFillColor(Color::Black);
        RectangleShape ButtonStart;
        ButtonStart.setSize(Vector2f(120, 25));
        ButtonStart.setOutlineColor(Color::Black);
        ButtonStart.setOutlineThickness(3);
        ButtonStart.setFillColor(Color(220, 220, 220));
        ButtonStart.setPosition(Vector2f(280, 270));
        Text TextStart(Font, "Start Demo", 20);
        TextStart.setPosition(Vector2f(290, 270));
        TextStart.setFillColor(Color::Black);
        RectangleShape ActiveGraphBorder;
        ActiveGraphBorder.setPosition(Vector2f(100, 60));
        ActiveGraphBorder.setSize(Vector2f(275, 50));
        ActiveGraphBorder.setFillColor(Color::Transparent);
        ActiveGraphBorder.setOutlineThickness(3);
        ActiveGraphBorder.setOutlineColor(Color::Black);
        RectangleShape TotalGraphBorder(ActiveGraphBorder);
        TotalGraphBorder.setPosition(Vector2f(100, 130));
        RectangleShape ActiveUsageGraph;
        ActiveUsageGraph.setPosition(Vector2f(100, 60));
        ActiveUsageGraph.setSize(Vector2f(275, 50));
        ActiveUsageGraph.setFillColor(Color::Cyan);
        RectangleShape TotalUsageGraph;
        TotalUsageGraph.setPosition(Vector2f(100, 130));
        TotalUsageGraph.setSize(Vector2f(275, 50));
        TotalUsageGraph.setFillColor(Color::Red);
        RectangleShape ButtonAllocate;
        ButtonAllocate.setSize(Vector2f(120, 25));
        ButtonAllocate.setOutlineColor(Color::Black);
        ButtonAllocate.setOutlineThickness(3);
        ButtonAllocate.setFillColor(Color(220, 220, 220));
        ButtonAllocate.setPosition(Vector2f(20, 240));
        RectangleShape ButtonDeallocate(ButtonAllocate);
        ButtonDeallocate.setPosition(Vector2f(150, 240));
        RectangleShape ButtonLeak(ButtonAllocate);
        ButtonLeak.setPosition(Vector2f(280, 240));
        Text ErrorMessage(Font, "", 15);
        ErrorMessage.setPosition(Vector2f(115, 270));
        ErrorMessage.setFillColor(Color::Red);
        Text TextAllocate(Font, "Allocate", 20);
        TextAllocate.setPosition(Vector2f(45, 240));
        TextAllocate.setFillColor(Color::Black);
        Text TextDeallocate(Font, "Deallocate", 20);
        TextDeallocate.setPosition(Vector2f(160, 240));
        TextDeallocate.setFillColor(Color::Black);
        Text TextLeak(Font, "Leak", 20);
        TextLeak.setPosition(Vector2f(320, 240));
        TextLeak.setFillColor(Color::Black);
        Text TextActiveMB(Font, "Active", 15);
        TextActiveMB.setPosition(Vector2f(25, 70));
        TextActiveMB.setFillColor(Color::Black);
        Text TextTotalMB(Font, "Total", 15);
        TextTotalMB.setPosition(Vector2f(25, 140));
        TextTotalMB.setFillColor(Color::Black);
        while (Window.isOpen()) { // After it finishes initial setup, it enters a while loop that does not end until the program is closed. This ensures that it can be used indefinitely, so long as the user wants to use it.
            while (const std::optional event = Window.pollEvent()) { // This "nested" while loop will detect any and all events that take place within this window, and perform a specific task if any events that this app makes use of are detected.
                if (event->is<Event::Closed>()) {
                    Window.close();
                }
                if (event->is<Event::MouseButtonPressed>()) {
                    ClickPos = Mouse::getPosition(Window);
                    Error = "";
                }
            }
            if (Activated) { // Activated section of the program. This is the code for the main page of this app.
                Window.clear(Color::Color(224, 224, 224));
                ActiveUsageGraph = GraphPercent(ActiveUsageGraph, CurrentMemoryInfo.GetActiveUsage(), 275);
                TotalUsageGraph = GraphPercent(TotalUsageGraph, CurrentMemoryInfo.GetTotalUsage(), 275);
                ErrorMessage.setString(Error);
                Window.draw(ButtonAllocate);
                Window.draw(ButtonDeallocate);
                Window.draw(ButtonLeak);
                Window.draw(TextAllocate);
                Window.draw(TextDeallocate);
                Window.draw(TextLeak);
                Window.draw(TextActiveMB);
                Window.draw(TextTotalMB);
                Window.draw(ErrorMessage);
                Window.draw(ActiveUsageGraph);
                Window.draw(ActiveGraphBorder);
                Window.draw(TotalUsageGraph);
                Window.draw(TotalGraphBorder);
                CurrentMemoryInfo = GetMemoryStats(CurrentMemoryInfo.GetActiveMB(), CurrentMemoryInfo.GetTotalMB());
                if (ClickPos != DefaultPos()) {
                    if (RectContains(ClickPos, ButtonAllocate)) {
                        CurrentMemoryInfo = ManageMemory(ALLOCATE, CurrentMemoryInfo);
                    }
                    else if (RectContains(ClickPos, ButtonDeallocate)) {
                        CurrentMemoryInfo = ManageMemory(DEALLOCATE, CurrentMemoryInfo);
                    }
                    else if (RectContains(ClickPos, ButtonLeak)) {
                        CurrentMemoryInfo = ManageMemory(LEAK, CurrentMemoryInfo);
                    }
                    ClickPos = DefaultPos();
                    Error = CurrentMemoryInfo.GetLastError();
                }
            }
            else { // Inactive section of the program. This is for showing the important info related to the app and it's purpose, and requires the user to click a specific button to move past it, to ensure they have a chance to read it all.
                Window.clear(Color::Color(224, 224, 224));
                Window.draw(TextInitialTitle);
                Window.draw(TextInitialInfo);
                Window.draw(ButtonStart);
                Window.draw(TextStart);
                if (ClickPos != DefaultPos()) {
                    if (RectContains(ClickPos, ButtonStart)) {
                        Activated = true;
                    }
                }
            }
            Window.display();
        }
    }
    AlloPtr = nullptr;
    return 0;
}