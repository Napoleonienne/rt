#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <ranges>
#include <cstdint>
#include <algorithm>
#include <cstddef>

constexpr uint32_t largeur = 800;
constexpr uint32_t hauteur = 600;



template <typename T>
class framebuffer {
public:
    using value_type = T;
    using size_type  = std::size_t;

    framebuffer(size_type l, size_type h, const T& default_val = T{})
        : donnees(l * h, default_val), m_largeur(l), m_hauteur(h) {}

    // Support complet du déplacement et de la copie
    framebuffer(const framebuffer&) = default;
    framebuffer& operator=(const framebuffer&) = default;
    framebuffer(framebuffer&&) noexcept = default;
    framebuffer& operator=(framebuffer&&) noexcept = default;
    ~framebuffer() = default;

    // --- Redimensionnement ---
    void redimensionner(size_type nouveau_l, size_type nouveau_h, const T& clear_val = T{}) {
        m_largeur = nouveau_l;
        m_hauteur = nouveau_h;
        std::views::cartesian_product(largeur(),auteur());
        




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

    // --- Métadonnées ---
    [[nodiscard]] constexpr size_type largeur() const noexcept { return m_largeur; }
    [[nodiscard]] constexpr size_type hauteur() const noexcept { return m_hauteur; }
    [[nodiscard]] constexpr size_type pitch() const noexcept { return m_largeur * sizeof(T); }
    [[nodiscard]] constexpr size_type size() const noexcept { return donnees.size(); }

private:
    std::vector<T> donnees;
    size_type m_largeur;
    size_type m_hauteur;
};



void write_color( const glm::vec3& pixel_color,uint32_t* scren) {
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;

    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);


}



inline void ecrit_couleur(uint32_t* raw, int taille_ligne, glm::ivec2 pos, glm::uvec4 couleur) {
    raw[pos.y * taille_ligne + pos.x] = static_cast<uint32_t>(couleur.r)        |
                                       (static_cast<uint32_t>(couleur.g) << 8)  |
                                       (static_cast<uint32_t>(couleur.b) << 16) |
                                       (static_cast<uint32_t>(couleur.a) << 24);
}

void aire_rec(glm::ivec3 &a, glm::ivec3 &b, glm::ivec3 &c) {
  auto base = b - a;
  auto hauteur = (base) / 2;
  int aire = (glm::length(base) * glm::length(c - hauteur)) / 2;
}



void ligne(uint32_t raw,int taile_ligne, glm::uvec3 a,glm::uvec3 b,glm::uvec4 couleur){
    auto step =0.1f;

    auto ran01 = std::views::iota(0, 100)| std::views::transform([step](int i) { return i * step; });
    int longe = glm::length(b-a);
    for (auto t :ran01 ) {
        
        
    
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
    bufferm<glm::uvec4> ab;
    uint32_t largeur = 800;
    uint32_t hauteur = 600;


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