#include "Map.hpp"
#include "View.hpp"
#include "Game.hpp"

int main() {
    View view;
    Map map(1);
    view.draw(map);

    return 0;
}