#include <rasteriser.hpp>










int main(int argc, char* argv[]){
    rasteriser rast;

    if (rast.run()){
        spdlog::error("la fenetre n'a pas pus initialiser");
    }
    return 0;
}





