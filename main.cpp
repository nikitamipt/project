#include "source/Game1.hpp"
#include "foo.hpp"
#include "source/MapDrawer.hpp"
#include "source/Game_with_Runners.hpp"
#include "source/Game_war.hpp"


#include <SFML/Graphics.hpp>



int main()
{
    srand(static_cast<unsigned int>(time(0)));
    int i = 0; int j = 1;
    while (j) {
        cout << "Write a number. if..\n1 - to draw map for runners\n2 - Game_with_Runners\n3 - War_Game\n0 - EXIT\n";
        cin >> i;
        if      (i == 1) { Draw();              j = 0;}
        else if (i == 2) { Game_with_Runners(); j = 0;} //здесь боты учатся бегать по лабиринту
        else if (i == 3) { Game_war();          j = 0;}
        else if (i == 0) { j = 0;}
        else {
            cout << "Wrong number. Read again\n\n";
        }
    }

  return 0;
}

