#include <cctype>

class DialogWin : public sciter::window {
private:

public:
    DialogWin(Controler* controler) : window(SW_ALPHA, { 0, 0 ,200, 330 }) {
    }
};