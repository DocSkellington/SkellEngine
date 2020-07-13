#pragma once

#include <array>
#include <stdexcept>

namespace engine {
    /**
     * \brief A matrix
     */
    template <typename T, std::size_t NRows, std::size_t NColumns>
    class Matrix {
    public:
        using dataType = std::array<std::array<T, NColumns>, NRows>;
        using iterator = typename dataType::iterator;
        using const_iterator = typename dataType::const_iterator;

    public:
        /**
         * \brief Initializes the matrix with the default value of type T
         */
        Matrix() requires std::is_constructible_v<T, void> || std::is_fundamental_v<T> {
            for (auto &column : m_data) {
                column.fill(T());
            }
        }
        
        /**
         * \brief Initializes the matrix such that all cells contain the given value
         */
        Matrix(T value) {
            for (auto &column : m_data) {
                column.fill(value);
            }
        }

        /**
         * \brief Initializes the matrix as given
         */
        Matrix(const std::array<std::array<T, NColumns>, NRows> &data) :
            m_data(data) {
        }

        /**
         * \brief Initializes the matrix from a list of values.
         * 
         * If the list's size exceeds the size of the matrix, an exception of type std::length_error is thrown.
         */
        Matrix(std::initializer_list<T> data) {
            if (data.size() > NRows * NColumns) {
                throw std::length_error("Error during matrix initialization: the provided initializer list contains more values (size = " + std::to_string(data.size()) + ") than can be stored in a matrix of size " + std::to_string(NRows) + " x " + std::to_string(NColumns));
            }
            std::size_t row = 0;
            std::size_t column = 0;
            for (const T &d : data) {
                m_data[row][column] = d;
                if (column == NColumns - 1) {
                    row++;
                    column = 0;
                }
                else {
                    column++;
                }
            }
        }

        template <typename B, std::size_t NRowsOther, std::size_t NColumnsOther>
            requires std::is_convertible_v<B, T>
        std::strong_ordering operator<=>(const Matrix<B, NRowsOther, NColumnsOther> &o) const {
            if (auto cmp = NRows <=> NRowsOther ; cmp != 0) {
                return cmp;
            }
            if (auto cmp = NColumns <=> NColumnsOther ; cmp != 0) {
                return cmp;
            }
            return m_data <=> o.m_data;
        }

        template <typename B, std::size_t NRowsOther, std::size_t NColumnsOther>
            requires std::is_convertible_v<B, T>
        bool operator==(const Matrix<B, NRowsOther, NColumnsOther> &o) const {
            // For some reasons, the compiler complains that == is not defined, even though <=> is
            // So, we explicitly add this overload
            return NRows == NRowsOther && NColumns == NColumnsOther && m_data == o.m_data;
        }

        constexpr std::size_t getNumberOfRows() const {
            return NRows;
        }

        constexpr std::size_t getNumberOfColumns() const {
            return NColumns;
        }

        T get(std::size_t row, std::size_t column) const {
            return m_data[row][column];
        }

        void set(std::size_t row, std::size_t column, T value) {
            m_data[row][column] = value;
        }

        void fillRow(std::size_t row, std::array<T, NColumns> data) {
            for (std::size_t column = 0 ; column < NColumns ; column++) {
                m_data[row][column] = data[column];
            }
        }

        void fillColumn(std::size_t column, std::array<T, NRows> data) {
            for (std::size_t row = 0 ; row < NRows ; row++) {
                m_data[row][column] = data[row];
            }
        }

        template <typename B = T, std::size_t NR = NRows, std::size_t NC = NColumns>
        std::enable_if_t<NR == 1, B> operator[] (std::size_t column) const {
            return m_data[0][column];
        }

        template <typename B = T, std::size_t NR = NRows, std::size_t NC = NColumns>
        std::enable_if_t<NC == 1, B> operator[] (std::size_t row) const {
            return m_data[row][0];
        }

        template <typename B = T, std::size_t NR = NRows, std::size_t NC = NColumns>
        std::enable_if_t<NC != 1 && NR != 1, Matrix<B, 1, NC>> operator[] (std::size_t row) {
            Matrix<B, 1, NC> m;
            m.fillRow(0, m_data[row]);
            return m;
        }

        template <typename B>
            requires std::is_convertible_v<B, T>
        Matrix<T, NRows, NColumns> operator+ (const Matrix<B, NRows, NColumns> &m) const {
            Matrix<T, NRows, NColumns> res;
            for (std::size_t row = 0 ; row < NRows ; row++) {
                for (std::size_t column = 0 ; column < NColumns ; column++) {
                    res.set(row, column, get(row, column) + m.get(row, column));
                }
            }
            return res;
        }

        template <typename B>
            requires std::is_convertible_v<B, T>
        Matrix<T, NRows, NColumns> operator+= (const Matrix<B, NRows, NColumns> &m) {
            for (std::size_t row = 0 ; row < NRows ; row++) {
                for (std::size_t column = 0 ; column < NColumns ; column++) {
                    set(row, column, get(row, column) + m.get(row, column));
                }
            }
            return *this;
        }

        template<typename B, std::size_t NRowsOther, std::size_t NColumnsOther>
            requires (NColumns == NRowsOther) && std::is_convertible_v<B, T>
        Matrix<T, NRows, NColumnsOther> operator* (const Matrix<B, NRowsOther, NColumnsOther> &m) const {
            Matrix<T, NRows, NColumnsOther> res;
            for (std::size_t row = 0 ; row < NRows ; row++) {
                for (std::size_t column = 0 ; column < NColumns ; column++) {
                    T sum = 0;
                    for (std::size_t k = 0 ; k < NColumns ; k++) {
                        sum += get(row, k) * m.get(k, column);
                    }
                    res.set(row, column, sum);
                }
            }
            return res;
        }

        constexpr iterator begin() noexcept {
            return m_data.begin();
        }

        constexpr const_iterator begin() const noexcept {
            return m_data.begin();
        }

        constexpr iterator end() noexcept {
            return m_data.end();
        }

        constexpr const_iterator end() const noexcept {
            return m_data.end();
        }

    private:
        dataType m_data;
    };

    template<typename T, std::size_t NRows, std::size_t NColumns>
    std::ostream& operator<<(std::ostream &os, const Matrix<T, NRows, NColumns> &matrix) {
        std::size_t i = 0;
        for (auto &line : matrix) {
            os << "(";
            for (std::size_t j = 0 ; j < line.size() ; j++) {
                os << std::to_string(line[j]);
                if (j != line.size() - 1) {
                    os << ", ";
                }
            }
            os << ")";
            i++;
            if (i != matrix.getNumberOfRows()) {
                os << "\n";
            }
        }
        return os;
    }

    /**
     * \brief Creates a diagonal matrix.
     * 
     * A diagonal matrix is a matrix where every value is null, except on the diagonal
     * \param diagonal The values to put in the diagonal
     */
    template <typename T, std::size_t N>
    Matrix<T, N, N> createDiagonalMatrix(const std::array<T, N> &diagonal) {
        Matrix<T, N, N> m;
        for (std::size_t i = 0 ; i < N ; i++) {
            m.set(i, i, diagonal[i]);
        }
        return m;
    }
}