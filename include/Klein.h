#pragma once

#include <sys/time.h>
#include <climits>
#include <iostream>
#include <cmath>
#include <cassert>
#include <semaphore.h>
#include <thread>
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>

/*Macros per il debug*/
#ifdef DEBUG
#  define KLEIN_DEBUG(name, val)  std::cout << (name) << ' ' << (val) << '\n'
#  define KLEIN_ASSERT(expr)      assert(expr)
#else
#  define KLEIN_DEBUG(name, val)  ;
#  define KLEIN_ASSERT(expr)      ;
#endif




namespace Klein
{
    /*Costanti globali*/
    inline constexpr int SCREEN_WIDTH        = 1920;
    inline constexpr int SCREEN_HEIGHT       = 1080;
    inline constexpr int MAX_PARTICLE_SIZE   = 50;
    inline constexpr int INT_ERROR_VALUE     = INT_MIN;

    /*Multithreading*/
    int         getMaxThreads();
    std::thread* getThreadPool();

    /**
     * @brief Struttura per la gestione del tempo
     */
    struct time_profile_t
    {
        timespec value{};       // Timestamp di origine (riferimento per i delta)
        float    delta = 0.f;   // Tempo trascorso dall'ultimo tick [secondi]
        float    speed = 1.f;   // Moltiplicatore di velocità (1.0 = tempo reale)
        float    time  = 0.f;   // Tempo accumulato dall'inizio [secondi]
    };

    
    /**
     * @brief Vettore con origine in (0,0)
     */
    struct vector_t
    {
        float x = 0.f;
        float y = 0.f;

        float magnitude() const { return std::sqrt(x*x + y*y); }
        
        vector_t normalized() 
        {
            float m = magnitude();
            KLEIN_ASSERT(m > 0.f);
            return {x/m, y/m};
        }

        /*Overload dei principali operatori*/
        vector_t  operator+(const vector_t& o) const { return {x+o.x, y+o.y}; }
        vector_t  operator-(const vector_t& o) const { return {x-o.x, y-o.y}; }
        vector_t  operator*(float s)           const { return {x*s,   y*s};   }
        vector_t& operator+=(const vector_t& o)      { x+=o.x; y+=o.y; return *this; }
        vector_t& operator*=(float s)                { x*=s;   y*=s;   return *this; }
    };


    /**
     * @brief Punto nello spazio 2D
     */
    struct point_t
    {
        float x = 0;
        float y = 0;
    };

    /*Tipi di Hitbox disponibili*/
    enum class HitboxType 
    { 
        Circle, 
        Rectangle,
        //Triangle ?? 
    };


    /**
     * @brief Rettangolo descritto sia come limiti (top, bottom, left, right) che come origine, larghezza e altezza
     */
    struct rectangle_t
    {
        float top    = 0;  // Bordo superiore (Y minore)
        float right  = 0;  // Bordo destro    (X maggiore)
        float bottom = 0;  // Bordo inferiore (Y maggiore)
        float left   = 0;  // Bordo sinistro  (X minore)

        float width()     const { return std::abs(right - left); }

        float height()    const { return std::abs(bottom - top); }

        float area()      const { return width() * height(); }

        float perimeter() const { return 2 * (width() + height()); }

        point_t origin() const {return {left, top};}

        /*Restituisce true se il punto p è contenuto nel rettangolo (bordi inclusi)*/
        bool contains(point_t p) const
        {
            return p.x >= left && p.x <= right
                && p.y >= top  && p.y <= bottom;
        }

        /*Espandono il rettangolo tenendo fisso il punto d'origine (left, top)*/
        
        void setHeight(float h)   {bottom = top + h;}
        void setWidth(float w)   {right = left + w;}

        void setCenter(point_t center)
        {
            float w = width();
            float h = height();

            top     = center.y - h/2;
            bottom  = center.y + h/2;
            left    = center.x - w/2;
            right   = center.x + w/2;
        }

        void setOrigin(point_t origin)
        {
            float w = width();
            float h = height();

            top     = origin.y;
            bottom  = top + h;
            left    = origin.x;
            right   = left + w;
        }

    };


    /**
     * @brief Cerchio descritto da centro e raggio.
     */
    struct circle_t
    {
        point_t center{};   // Centro del cerchio
        float   radius = 0; // Raggio in pixel

        float area()            const { return M_PI * radius * radius; }

        float circumference()   const { return 2.f * M_PI * radius; }

        /*Restituisce true se il punto p è contenuto nel cerchio (bordo incluso)*/
        bool contains(point_t p) const
        {
            const float dx = p.x - center.x;
            const float dy = p.y - center.y;
            return (dx*dx + dy*dy) <= (radius*radius);
        }
    };

    
    #define BENCHMARK
    #ifdef BENCHMARK
        /**
         * @brief Utility per la misurazione del tempo di esecuzione.
         */
        struct Benchmark
        {
            private:
                static std::chrono::steady_clock::time_point s_start;
                static std::chrono::steady_clock::time_point s_end;

            public:
                static void begin() { s_start = std::chrono::steady_clock::now(); }
                static void end()   { s_end   = std::chrono::steady_clock::now(); }

                static long long getMicroseconds()
                {
                    return std::chrono::duration_cast<std::chrono::microseconds>(s_end - s_start).count();
                }

                static void print(const char* name)
                {
                    std::cout << "Time [" << name << "]: " << getMicroseconds() << " us\n";
                }

        };

        #define BEGIN_TIME  (Klein::Benchmark::begin())
        #define END_TIME    (Klein::Benchmark::end())
        #define GET_TIME    (Klein::Benchmark::getMicroseconds())
        #define PRINT_TIME(name) (Klein::Benchmark::print(name))
    #else
        #define BEGIN_TIME          ;
        #define END_TIME            ;
        #define GET_TIME            0
        #define PRINT_TIME(name)    ;
    #endif

} // namespace Klein