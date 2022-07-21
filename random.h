
#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <chrono>
#include <limits>
#include <random>
#include <vector>

namespace non_deterministic {

    /**
     * @brief Restituisce un numero intero nell'intervallo [0, end]
     *
     * @param end estremo superiore (default: più grande intero rappresentabile)
     * @return int intero casuale
     */
    static int random_unsigned_int(const int &end = std::numeric_limits<int>::max()) {
        // la sequenza non è riproducibile
        static auto seed = std::random_device{}();

        // Mersenne Twister random engine
        static std::mt19937 urbg{seed};

        // generate random unsigned int starting from 0
        std::uniform_int_distribution<int> distr{0, end};

        int rand = distr(urbg);

        return rand;

    }

    /**
     * @brief Restituisce un numero intero nell'intervallo [start, end]
     *
     * @param start estremo inferiore (default: più piccolo intero rappresentabile)
     * @param end estremo superiore (default: più grande intero rappresentabile)
     * @return int intero casuale
     */
    static int random_int(const int& start = std::numeric_limits<int>::min(),
                          const int& end = std::numeric_limits<int>::max()) {
        // la sequenza non è riproducibile
        static auto seed = std::random_device{}();

        // Mersenne Twister random engine
        static std::mt19937 urbg{seed};

        // generate random unsigned int starting from start
        std::uniform_int_distribution<int> distr{start, end};

        int rand = distr(urbg);

        return rand;

    }

    /**
     * @brief Restituisce un numero in virgola mobile nell'intervallo [start, end]
     *
     * @param start estremo inferiore (default: più piccolo double rappresentabile)
     * @param end estremo superiore (default: più grande double rappresentabile)
     * @return double numero in virgola mobile casuale
     */
    static double random_double(const double& start = std::numeric_limits<double>::min(),
                                const double& end = std::numeric_limits<double>::max()) {
        // la sequenza non è riproducibile
        static auto seed = std::random_device{}();

        // Mersenne Twister random engine
        static std::mt19937 urbg(seed);

        // generate random double in [start, end]
        static std::uniform_real_distribution<double> distr{start, end};
        
        double rand = distr(urbg);
        
        return rand;

    }

    /**
     * @brief Genera, in modo non deterministico, un vettore
     *        di numeri interi di dimensione size.
     *        Tutti i numeri restituiti hanno un segno e
     *        provengono da una distribuzione uniforme
     *
     * @param size dimensione del vettore
     * @return std::vector<int> vettore di numeri casuali
     */
    static std::vector<int> random_int_vector(const int& size) {
        std::vector<int> v(size);

        // la sequenza non è riproducibile
        static auto seed = std::random_device{}();

        // Mersenne Twister random engine
        static std::mt19937 urbg{seed};

        // generate random unsigned int starting from start
        std::uniform_int_distribution<int> distr{
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max()
        };

        for (int i = 0; i < size; i = i + 1) {
            v[i] = distr(urbg);
        }

        return v;
    }

    /**
     * @brief Genera, in modo non deterministico, un vettore
     *        di numeri interi ordinato di dimensione size.
     *        Tutti i numeri restituiti hanno un segno e
     *        provengono da una distribuzione uniforme
     *
     * @param size dimensione del vettore
     * @return std::vector<int> vettore di numeri casuali
     */
    static std::vector<int> random_sorted_int_vector(const int& size) {
        std::vector<int> v(size);

        // la sequenza non è riproducibile
        static auto seed = std::random_device{}();

        // Mersenne Twister random engine
        static std::mt19937 urbg{seed};

        // generate random unsigned int starting from start
        std::uniform_int_distribution<int> distr{
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max()
        };

        v[0] = distr(urbg);

        int i = 1;
        while (i < size) {
            int rand = distr(urbg);
            if (rand >= v[i - 1]) {
                v[i] = rand;
                i = i + 1;
            }
        }

        return v;
    }

    /**
     * @brief Genera, in modo non deterministico, un vettore
     *        di numeri in virgola mobile di dimensione size.
     *        Tutti i numeri restituiti hanno un segno e
     *        provengono da una distribuzione uniforme
     *
     * @param size dimensione del vettore
     * @return std::vector<int> vettore di numeri casuali
     */
    static std::vector<double> random_double_vector(const int &size) {
        std::vector<double> v(size);

        // la sequenza non è riproducibile
        static auto seed = std::random_device{}();

        // Mersenne Twister random engine
        static std::mt19937 urbg{seed};

        // generate random double in [-1, 1]
        static std::uniform_real_distribution<double> distr{-1, 1};
        // generate int in [-∞, +∞]
        std::uniform_int_distribution<int> distr1{};

        for (int i = 0; i < size; i = i + 1) {
            double rand = distr(urbg) * distr1(urbg);
            v[i] = rand;
        }

        return v;
    }

}

namespace deterministic {

    /**
     * @brief Restituisce un numero intero nell'intervallo [0, end]
     *
     * @param end estremo superiore (default: più grande intero rappresentabile)
     * @return int intero casuale
     */
    static int random_unsigned_int(const int& end = std::numeric_limits<int>::max()) {
        // la sequenza è riproducibile
        static auto seed = std::chrono::system_clock::now().time_since_epoch().count();

        // Mersenne Twister random engine
        static std::mt19937 urbg(seed);

        // generate random unsigned int starting from 0
        std::uniform_int_distribution<int> distr{0, end};

        int rand = distr(urbg);

        return rand;

    }

    /**
     * @brief Restituisce un numero intero nell'intervallo [start, end]
     *
     * @param start estremo inferiore (default: più piccolo intero rappresentabile)
     * @param end estremo superiore (default: più grande intero rappresentabile)
     * @return int intero casuale
     */
    static int random_int(const int& start = std::numeric_limits<int>::min(),
                          const int& end = std::numeric_limits<int>::max()) {
        // la sequenza è riproducibile
        static auto seed = std::chrono::system_clock::now().time_since_epoch().count();

        // Mersenne Twister random engine
        static std::mt19937 urbg(seed);

        // generate random unsigned int starting from start
        std::uniform_int_distribution<int> distr{start, end};

        int rand = distr(urbg);

        return rand;

    }

    /**
     * @brief Restituisce un numero in virgola mobile nell'intervallo [start, end]
     *
     * @param start estremo inferiore (default: più piccolo double rappresentabile)
     * @param end estremo superiore (default: più grande double rappresentabile)
     * @return double numero in virgola mobile casuale
     */
    static double random_double(const double& start = std::numeric_limits<double>::min(),
                                const double& end = std::numeric_limits<double>::max()) {

        // la sequenza è riproducibile
        static auto seed = std::chrono::system_clock::now().time_since_epoch().count();

        // Mersenne Twister random engine
        static std::mt19937 urbg(seed);

        // generate random double in [start, end]
        static std::uniform_real_distribution<double> distr{start, end};
        
        double rand = distr(urbg);
        
        return rand;

    }

    /**
     * @brief Genera, in modo deterministico, un vettore
     *        di numeri interi di dimensione size.
     *        Tutti i numeri restituiti hanno un segno e
     *        provengono da una distribuzione uniforme
     *
     * @param size dimensione del vettore
     * @return std::vector<int> vettore di numeri casuali
     */
    static std::vector<int> random_int_vector(const int& size) {
        std::vector<int> v(size);

        // la sequenza è riproducibile
        static auto seed = std::chrono::system_clock::now().time_since_epoch().count();

        // Mersenne Twister random engine
        static std::mt19937 urbg(seed);

        // generate random unsigned int starting from start
        std::uniform_int_distribution<int> distr{
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max()
        };

        for (int i = 0; i < size; i = i + 1) {
            v[i] = distr(urbg);
        }

        return v;
    }

    /**
     * @brief Genera, in modo deterministico, un vettore
     *        di numeri interi ordinato di dimensione size.
     *        Tutti i numeri restituiti hanno un segno e
     *        provengono da una distribuzione uniforme
     *
     * @param size dimensione del vettore
     * @return std::vector<int> vettore di numeri casuali
     */
    static std::vector<int> random_sorted_int_vector(const int& size) {
        std::vector<int> v(size);

        // la sequenza è riproducibile
        static auto seed = std::chrono::system_clock::now().time_since_epoch().count();

        // Mersenne Twister random engine
        static std::mt19937 urbg(seed);

        // generate random unsigned int starting from start
        std::uniform_int_distribution<int> distr{
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max()
        };

        v[0] = distr(urbg);

        int i = 1;
        while (i < size) {
            int rand = distr(urbg);

            if (rand >= v[i - 1]) {
                v[i] = rand;
                i = i + 1;
            }
        }

        return v;
    }

    /**
     * @brief Genera, in modo deterministico, un vettore
     *        di numeri in virgola mobile di dimensione size.
     *        Tutti i numeri restituiti hanno un segno e
     *        provengono da una distribuzione uniforme
     *
     * @param size dimensione del vettore
     * @return std::vector<int> vettore di numeri casuali
     */
    static std::vector<double> random_double_vector(const int& size) {
        std::vector<double> v(size);

        // la sequenza è riproducibile
        static auto seed = std::chrono::system_clock::now().time_since_epoch().count();

        // Mersenne Twister random engine
        static std::mt19937 urbg(seed);

        // generate random double in [-1, 1]
        static std::uniform_real_distribution<double> distr{-1, 1};
        // generate int in [-∞, +∞]
        std::uniform_int_distribution<int> distr1{};

        for (int i = 0; i < size; i = i + 1) {
            double rand = distr(urbg) * distr1(urbg);
            v[i] = rand;
        }

        return v;
    }

}

#endif // __RANDOM_H__
