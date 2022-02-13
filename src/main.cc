#include"error_codes.h"
#include"window_manager.h"

int main(){
    window_manager window;

    if(auto error_code = window.init()){
        return error_code;
    }

    window.handle();

    return NO_ERROR;
}
