export module vector;

import std;

// vec3 — 3-component double-precision vector used throughout the ray-tracer
export class vec3 {
public:
    std::array<double, 3> e{0.0, 0.0, 0.0};

    constexpr vec3() noexcept = default;
    constexpr vec3(double e0, double e1, double e2) noexcept : e{e0, e1, e2} {}

    [[nodiscard]] constexpr double x() const noexcept { return e[0]; }
    [[nodiscard]] constexpr double y() const noexcept { return e[1]; }
    [[nodiscard]] constexpr double z() const noexcept { return e[2]; }

    [[nodiscard]] constexpr vec3 operator-() const noexcept { return {-e[0], -e[1], -e[2]}; }
    [[nodiscard]] constexpr double operator[](int i) const { return e[i]; }
    [[nodiscard]] constexpr double& operator[](int i) { return e[i]; }

    bool operator==(const vec3& other) const {
        return this->e == other.e;
    }

    constexpr vec3& operator+=(const vec3& v) noexcept {
        e[0] += v.e[0]; e[1] += v.e[1]; e[2] += v.e[2];
        return *this;
    }

    constexpr vec3& operator*=(double t) noexcept {
        e[0] *= t; e[1] *= t; e[2] *= t;
        return *this;
    }

    constexpr vec3& operator/=(double t) noexcept {
        return *this *= 1.0 / t;
    }

    [[nodiscard]] constexpr double length_squared() const noexcept {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    [[nodiscard]] double length() const noexcept {
        return std::sqrt(length_squared());
    }
};

export using point3 = vec3;

// Structured bindings support
export template<std::size_t I>
[[nodiscard]] constexpr double get(const vec3& v) noexcept { return v.e[I]; }

// std namespace specialisations must be visible to importers
namespace std {
    export template<> struct tuple_size<vec3> : integral_constant<size_t, 3> {};
    export template<size_t I> struct tuple_element<I, vec3> { using type = double; };
}

// Global arithmetic operators
export [[nodiscard]] constexpr vec3 operator+(const vec3& u, const vec3& v) noexcept {
    return {u.x() + v.x(), u.y() + v.y(), u.z() + v.z()};
}

export [[nodiscard]] constexpr vec3 operator-(const vec3& u, const vec3& v) noexcept {
    return {u.x() - v.x(), u.y() - v.y(), u.z() - v.z()};
}

export [[nodiscard]] constexpr vec3 operator*(const vec3& u, const vec3& v) noexcept {
    return {u.x() * v.x(), u.y() * v.y(), u.z() * v.z()};
}

export [[nodiscard]] constexpr vec3 operator*(double t, const vec3& v) noexcept {
    return {t * v.x(), t * v.y(), t * v.z()};
}

export [[nodiscard]] constexpr vec3 operator*(const vec3& v, double t) noexcept {
    return t * v;
}

export [[nodiscard]] constexpr vec3 operator/(const vec3& v, double t) noexcept {
    return (1.0 / t) * v;
}

export [[nodiscard]] constexpr double dot(const vec3& u, const vec3& v) noexcept {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

export [[nodiscard]] constexpr vec3 cross(const vec3& u, const vec3& v) noexcept {
    return {u.y() * v.z() - u.z() * v.y(),
            u.z() * v.x() - u.x() * v.z(),
            u.x() * v.y() - u.y() * v.x()};
}

export [[nodiscard]] inline vec3 unit_vector(const vec3& v) noexcept {
    return v / v.length();
}

// Formatting support
export template <>
struct std::formatter<vec3> : std::formatter<std::string_view> {
    auto format(const vec3& v, format_context& ctx) const {
        return std::format_to(ctx.out(), "{:.0f} {:.0f} {:.0f}", v.x(), v.y(), v.z());
    }
};

export inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << std::format("{}", v);
}
