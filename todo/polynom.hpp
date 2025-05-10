class Polynom {
public:

    Polynom(const std::initializer_list<double> coeff) : grad(coeff.size()-1), coeff(coeff){}
    ~Polynom() = default;

    Polynom& operator+=(const Polynom& other) {
        const int lim = std::min(other.grad, this->grad) +1;
        for(int i= 0; i < lim; i++) {
            this->coeff[i] += other.coeff[i];
        }
        if ( other.grad > this->grad) {
            this->coeff.resize(other.grad +1 );
            for(int i = lim; i < other.grad +1 ; i++) {
                this->coeff[i] = other.coeff[i];
            }
            this->grad = other.grad;
        }
        return *this;
    }

    Polynom& operator-=(const Polynom& other) {
        const int lim = std::min(other.grad, this->grad) +1;
        for(int i= 0; i < lim; i++) {
            this->coeff[i] -= other.coeff[i];
        }
        return *this;
    }

    Polynom& operator*=(const Polynom& other) {
        std::vector<double> newgrads = std::vector<double>(other.grad*this->grad + 1,0);
        const int tsize = this->grad + 1;
        const int osize = other.grad + 1;
        for(int i = 0; i < tsize; ++i) {
            for(int j = 0; j < osize; ++j) {
                newgrads[i+j] += this->coeff[i] * other.coeff[j];
            }
        }
        this->coeff.swap(newgrads);
        return * this;
    }

    double operator()(double x) const {
        const int size = this->grad +1;
        double sum = 0;
        for(int i = size; i >= 0; --i) {
            sum = sum*x + coeff[i];
        }
        return sum;
    }

    int grad;
    std::vector<double> coeff;
private:


};

inline std::ostream & operator<<(std::ostream & stream, const Polynom poly) {
    const int size = poly.grad;
    for(int i = size; i > 0 ; --i) {
        stream << poly.coeff[i] << "*x^" << i << " + ";
    }
    stream << poly.coeff[0];
    return stream;
}
