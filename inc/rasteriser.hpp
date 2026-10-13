#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <ranges>
#include <cstdint>
#include <algorithm>
#include <cstddef>
#include <cmath>
#include "camera.hpp"
constexpr uint32_t LARGEUR = 800;
constexpr uint32_t HAUTEUR = 600;



template <typename T>
class framebuffer {
public:
    using value_type = T;
    using size_type  = std::size_t;
    framebuffer(){}

    framebuffer(size_type l, size_type h, const T& default_val = T{})
        : donnees(l * h, default_val), m_largeur(l), m_hauteur(h) {}

    framebuffer(const framebuffer&) = delete;
    framebuffer& operator=(const framebuffer&) = default;
    framebuffer(framebuffer&&) noexcept = default;
    framebuffer& operator=(framebuffer&&) noexcept = default;
    ~framebuffer() = default;

    // --- Redimensionnement ---
    void redimensionner(size_type nouveau_l, size_type nouveau_h, const T& clear_val = T{}) {
        auto av_ch= iterateur();
        m_largeur = nouveau_l;
        m_hauteur = nouveau_h;

        auto ap_ch = iterateur();
        for (auto [y,x] :ap_ch) {
            

        
        }

        donnees.assign(m_largeur * m_hauteur, clear_val);
    }

    // --- Réinitialisation rapide de début de trame ---
    void clear(const T& valeur) noexcept {
        std::ranges::fill(donnees, valeur);
    }

    // --- Accesseurs scalaires (C++23 multi-subscript) ---
    [[nodiscard]] constexpr T& operator[](size_type x, size_type y) noexcept {
        return donnees[y * m_largeur + x];
    }

    [[nodiscard]] constexpr const T& operator[](size_type x, size_type y) const noexcept {
        return donnees[y * m_largeur + x];
    }

    // --- Accesseurs discrets GLM ---
    [[nodiscard]] constexpr T& operator[](glm::ivec2 p) noexcept {
        return donnees[static_cast<size_type>(p.y) * m_largeur + static_cast<size_type>(p.x)];
    }

    [[nodiscard]] constexpr const T& operator[](glm::ivec2 p) const noexcept {
        return donnees[static_cast<size_type>(p.y) * m_largeur + static_cast<size_type>(p.x)];
    }
    [[nodiscard]] constexpr const T& operator[](glm::vec2 p) const noexcept {
        if( (-1 <= p.x || p.x <= 1) && (-1<= p.y|| p.y <= 1) )
            return donnees[static_cast<size_type>((p.y+hauteur()/2)*hauteur()) * m_largeur + static_cast<size_type>((p.x+largeur()/2)*largeur())];
        else{
            spdlog::error("si la position est un vec2 alors sa doit etre entre -1 et 1" );
            return T(0);
        }
    }

    // --- Accès direct à la mémoire contiguë ---
    [[nodiscard]] T* data() noexcept { return donnees.data(); }
    [[nodiscard]] const T* data() const noexcept { return donnees.data(); }

    // --- Vues std::span ---
    // Vue 1D sur l'ensemble du tampon
    [[nodiscard]] std::span<T> span() noexcept {
        return std::span<T>(donnees);
    }

    [[nodiscard]] std::span<const T> span() const noexcept {
        return std::span<const T>(donnees);
    }

    // Vue 1D sur une ligne précise (utile pour découper le travail par thread)
    [[nodiscard]] std::span<T> row(size_type y) noexcept {
        return std::span<T>(donnees.data() + (y * m_largeur), m_largeur);
    }

    [[nodiscard]] std::span<const T> row(size_type y) const noexcept {
        return std::span<const T>(donnees.data() + (y * m_largeur), m_largeur);
    }

     /*
     renvoir un iterateur 
     odre d'abord y puis x
        
        
        */
    auto iterateur(){
    
        
        auto y_range = std::views::iota(0, static_cast<int>(hauteur()));
        auto x_range = std::views::iota(0, static_cast<int>(largeur()));

        return std::views::cartesian_product(y_range,x_range) ;
    }
    // --- Métadonnées ---
    [[nodiscard]] constexpr  int largeur() const noexcept { return m_largeur; }
    [[nodiscard]] constexpr  int hauteur() const noexcept { return m_hauteur; }
    [[nodiscard]] constexpr  int pitch() const noexcept { return m_largeur * sizeof(T); }
    [[nodiscard]] constexpr  int size() const noexcept { return donnees.size(); }



    void set_hauteur(int nv){
        m_hauteur = nv;
    }
     void set_largeur(int nv){
        m_largeur = nv;
    }

private:
    std::vector<T> donnees;
    size_type m_largeur =0;
    size_type m_hauteur =0;
};


glm::vec4 interpolation(glm::vec4 a ,glm::vec4 b){
    glm::vec4 dist = b-a;
    glm::vec4 res =b;



    return res;
}

void Depth_Test(
    framebuffer<glm::vec4> &buff,
    framebuffer<float> &zbuffering,glm::vec3 pos){

    


    
        return;
}



uint32_t convertir( const glm::vec4& pixel_color,glm::vec3 pos) {
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;
    uint32_t res;
   
    int rbyte = uint32_t(255.999 * r) ;
    int gbyte = uint32_t(255.999 * r);
    int bbyte = uint32_t(255.999 * r);
    int alpha = uint32_t(255.999 * r);

    res = rbyte|gbyte << 8|bbyte << 16 |alpha << 24;


    return res;

}

void dump(framebuffer<glm::vec4> &buff,framebuffer<float> &zbuffering,uint32_t* raw){


    for (auto [y,x]:buff.iterateur()) {
        uint32_t couleur = convertir( buff[x,y],glm::vec3(x,y,1));
        raw[y*buff.largeur()+x] = couleur ;
    }
}



void ecrit_couleur(uint32_t* raw, int taille_ligne, glm::ivec2 pos, glm::uvec4 couleur) {
    raw[pos.y * taille_ligne + pos.x] = static_cast<uint32_t>(couleur.r)        |
                                       (static_cast<uint32_t>(couleur.g) << 8)  |
                                       (static_cast<uint32_t>(couleur.b) << 16) |
                                       (static_cast<uint32_t>(couleur.a) << 24);
}





void Bresenha_line_drawing( glm::ivec3 vec1,glm::ivec3 vec2,glm::vec4 couleur,framebuffer<glm::vec4> &buff){
    //auto ran01 = std::views::iota(0, 100)| std::views::transform([](int i) { return i * 0.01f; });
    int ax,ay,bx,by;
    ax =vec1.x;
    ay = vec1.y;
    bx = vec2.x;
    by = vec2.y;

    bool steep = std::abs(ax-bx) < std::abs(ay-by);
    if (steep) { // if the line is steep, we transpose the image

        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax>bx) { // make it left−to−right
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int y = ay;
    int ierror = 0;
    for (int x=ax; x<=bx; x++) {
        if (steep) // if transposed, de−transpose
            buff[y,x] =couleur;
        else
            buff[x,y] =couleur;
        ierror += 2 * std::abs(by-ay);
        y += (by > ay ? 1 : -1) * (ierror > bx - ax);
        ierror -= 2 * (bx-ax)   * (ierror > bx - ax);
    }

}

    



void triangle(uint32_t raw,int taile_ligne, glm::uvec3 a,glm::uvec3 b,glm::uvec3 c,glm::uvec4 couleur){


}



class rasteriser {
public:
    rasteriser() = default;


    ~rasteriser() {
        liberer();
    }

    // Empêcher les copies pour éviter les doubles libérations de pointeurs SDL
    rasteriser(const rasteriser&) = delete;
    rasteriser& operator=(const rasteriser&) = delete;

    bool run() {
        if (!init()) {
            spdlog::error("Erreur lors de l'initialisation");
            liberer();
            return false;
        }

        spdlog::info("Démarrage de la boucle principale");
        mainloop();
        spdlog::info("Fin normale du programme");
        liberer();
        return true;
    }

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* screen_texture = nullptr;
    SDL_Surface* cpu_surface = nullptr;
    uint32_t largeur = LARGEUR;
    uint32_t hauteur = HAUTEUR;
    framebuffer<glm::vec4> frame1 =framebuffer<glm::vec4>(largeur,hauteur) ;
    framebuffer<float> zbuffer =framebuffer<float>(largeur,hauteur,-INFINITY) ;



    bool running = true;
    SDL_Event event{};

    bool init() {
        // 1. Initialiser le sous-système vidéo en premier
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            spdlog::error("Erreur SDL_Init : {}", SDL_GetError());
            return false;
        }

        // 2. Création de la fenêtre
        window = SDL_CreateWindow(
            "Rasterizer CPU - SDL3 (Option 2: SDL_Surface)",
            largeur,
            hauteur,
            SDL_WINDOW_RESIZABLE
        );
        if (!window) {
            spdlog::error("Erreur SDL_CreateWindow : {}", SDL_GetError());
            return false;
        }

        // 3. Création du renderer matériel
        renderer = SDL_CreateRenderer(window, nullptr);
        if (!renderer) {
            spdlog::error("Erreur SDL_CreateRenderer : {}", SDL_GetError());
            return false;
        }

        // 4. Texture GPU réceptrice pour l'affichage
        screen_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            largeur,
            hauteur
        );
        if (!screen_texture) {
            spdlog::error("Erreur SDL_CreateTexture : {}", SDL_GetError());
            return false;
        }

        // 5. Surface en RAM système pour le dessin CPU pur
        cpu_surface = SDL_CreateSurface(
            largeur,
            hauteur,
            SDL_PIXELFORMAT_RGBA32
        );
        if (!cpu_surface) {
            spdlog::error("Erreur SDL_CreateSurface : {}", SDL_GetError());
            return false;
        }

        // 6. Gestion du ratio d'aspect (letterboxing)
        SDL_SetRenderLogicalPresentation(
            renderer,
            largeur,
            hauteur,
            SDL_LOGICAL_PRESENTATION_LETTERBOX
        );

        return true;
    }

    void mainloop() {
        auto* pixels = static_cast<uint32_t*>(cpu_surface->pixels);
        const int taille_ligne = cpu_surface->pitch / sizeof(uint32_t);

        auto y_range = std::views::iota(0, static_cast<int>(hauteur));
        auto x_range = std::views::iota(0, static_cast<int>(largeur));

        while (running) {
            // Traitement des événements
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    running = false;
                }
            }
            // --- Étape A : Rendu logiciel dans la RAM CPU ---
            for (auto [y, x] : std::views::cartesian_product(y_range, x_range)) {
                glm::uvec4 couleur(
                    static_cast<uint32_t>((x * 255) / largeur),
                    static_cast<uint32_t>((y * 255) / hauteur),
                    128,
                    255
                );

                ecrit_couleur(pixels, taille_ligne, glm::ivec2(x, y), couleur);
       


            }

            cleanbuffer();
        }
    }

    void cleanbuffer() {
        SDL_UpdateTexture(
            screen_texture,
            nullptr,
            cpu_surface->pixels,
            cpu_surface->pitch
        );

        // Présentation à l'écran
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, screen_texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    void liberer() {
        if (cpu_surface) {
            SDL_DestroySurface(cpu_surface);
            cpu_surface = nullptr;
        }
        if (screen_texture) {
            SDL_DestroyTexture(screen_texture);
            screen_texture = nullptr;
        }
        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        SDL_Quit();
    }
};