#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <windows.h>
#include <sstream>

template <typename T>
std::string to_string_workaround(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

class Stopwatch
{
public:
    Stopwatch() : start_time_(std::chrono::steady_clock::now()) {}

    std::chrono::seconds elapsed_time() const
    {
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time_);
    }

private:
    std::chrono::steady_clock::time_point start_time_;
};

class UserInterface
{
public:
    UserInterface() { }

    bool key_pressed() const { return _kbhit(); }

    void clear_screen()
    {
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count;
        DWORD cellCount;
        COORD homeCoords = { 0, 0 };

        if (hStdOut == INVALID_HANDLE_VALUE) return;

        if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
        cellCount = csbi.dwSize.X * csbi.dwSize.Y;

        FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords, &count);
        FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count);
        SetConsoleCursorPosition(hStdOut, homeCoords);
    }

    void display(const std::string& message)
    {
        clear_screen();
        std::cout << message;
    }
};

int main()
{
    try
    {
        UserInterface ui;
        Stopwatch stopwatch;

        ui.display("Press any key to stop the stopwatch");

        while (!ui.key_pressed())
        {
            std::string message = "Elapsed time: " + to_string_workaround(stopwatch.elapsed_time().count()) + " seconds\n";
            ui.display(message);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        static_cast<void>(_getch());
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
