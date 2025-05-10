
void VecFillSinus(NaiveVector &a) {
    const size_t size = a.size();
    for(int i = 0; i < size; ++i) {
        a[i] = sin(2*M_PI*(i/(double(size-1))));
    }
}

void VecFillCosinus(StdVector &a) {
    const size_t size = a.size();
    for(size_t i = 0; i < size; ++i) {
        a[i] = cos(2*M_PI*i/(double(size-1)));
    }
}

void VecPrint() {
    // assumtion size > 0
    const size_t size = this->size();
    std::cout << "[ " << this->operator[](0);
    for (size_t i = 1; i < size; ++i) {
        std::cout << ", " << this->operator[](i);
    }
    std::cout << "]" << std::endl;
}

double VecScalarProduct(const StdVector &a, const StdVector &b) {
    const size_t size = a.size();
    double sum = 0.0;
    for(size_t i = 0; i < size; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

double VecNormMax(const StdVector &a) {
    // assumtion a.size > 0
    const size_t size = a.size();
    double max = fabs(a[0]);
    for(size_t i = 1; i < size; ++i) {
        double value = fabs(a[i]);
        if (value > max) {
            max = value;
        }
    }
    return max;
}

double VecNormEuclidian(const StdVector &a) {
    const size_t size = a.size();
    double sum = 0.0;
    for(size_t i = 0; i < size; ++i) {
        sum += a[i]*a[i];
    }
    return std::sqrt(sum);
}

double VecNormSum(const StdVector &a) {
    double sum = 0.0;
    const size_t size = a.size();
    for(size_t i = 0; i < size; ++i) {
        sum += fabs(a[i]);
    }
    return sum;
}

void StdVector::copy(const StdVector &other) {
    const size_t size = this->size();
    for (size_t i = 0; i < size; ++i) {
        this->operator[](i) = other[i];
    }
}


double StdVector::sdot(const StdVector &other) const {
    const size_t size = this->size();
    double sum = this->get_value(0)*other.get_value(0);
    double compensation = 0.0;

    for (size_t i = 1; i < size; ++i) {
        const double y = this->get_value(i) * other[i] - compensation;
        const double t = sum + y;
        compensation = (t - sum) - y;
        sum = t;
    }
    return sum;
}


double StdVector::dot(const StdVector &other) const {
    const size_t size = this->size();
    double sum = 0.0;
    for (size_t i = 0; i < size; ++i) {
        sum += this->operator[](i) * other[i];
    }
    return sum;
}

double StdVector::norm_abs() const {
    double sum = 0.0;
    const size_t size = this->size();
    for (size_t i = 0; i < size; ++i) {
        sum += fabs(this->operator[](i));
    }
    return sum;
}

double StdVector::norm_l2() const {
    const size_t size = this->size();
    double sum = 0.0;
    for (size_t i = 0; i < size; ++i) {
        sum += this->operator[](i) * this->operator[](i);
    }
    return std::sqrt(sum);
}

double StdVector::norm_max() const {
    const size_t size = this->size();
    double max = fabs(this->operator[](0));
    for (size_t i = 1; i < size; ++i) {
        double value = fabs(this->operator[](i));
        if (value > max) {
            max = value;
        }
    }
    return max;
}

void StdVector::swap(StdVector &other) noexcept {
    this->data_.swap(other.data_);
    std::swap(this->size_,other.size_);
}



void VecAdd(NaiveVector & result, NaiveVector a, NaiveVector b) {
    size_t size = a.size();
    for(int i = 0; i < size; ++i) {
        result[i] = a[i] + b[i];
    }
}

void VecSub(StdVector & result, const StdVector &a,const StdVector &b) {
    const size_t size = a.size();
    for(size_t i = 0; i < size; ++i) {
        result[i] = a[i] - b[i];
    }
}


size_t StdVector::id_max() const {
    const size_t size = this->size();
    double max = fabs(this->get_value(0));
    size_t max_idx = 0;
    for (size_t i = 1; i < size; ++i) {
        double value = fabs(this->get_value(i));
        if (value > max) {
            max_idx = i;
            max = value;
        }
    }
    return max_idx;
}

void StdVector::gemv(const double alpha, const StdMatrix &A, const StdVector &x, const double beta) {
    const size_t rows = A.rows();
    const size_t cols = A.cols();
    for(size_t i = 0; i < rows; ++i) {
        double sum = 0.0;
        for(size_t j = 0; j < cols; ++j) {
             sum += A.get_value(i,j)*x[j];
        }
        this->operator[](i) = alpha*sum + beta*this->operator[](i);
    }
};


void VecHadamarProduct(StdVector & result, const StdVector& a, const StdVector &b) {
    const size_t size = a.size();
    for(size_t i = 0; i < size; ++i) {
        result[i] = a[i] * b[i];
    }
}












double StdVector::sum() const {
    const size_t size = this->size();
    double sum = 0.0;
    for (size_t i = 0; i < size; ++i) {
        sum += this->operator[](i);
    }
    return sum;
}







void VecHadamarDiv(StdVector&  result, const StdVector &a, const StdVector &b) {
    const size_t size = a.size();
    for(size_t i = 0; i < size; ++i) {
        result[i] = a[i] / b[i];
    }
}

void VecAxpy(NaiveVector & result, double alpha, NaiveVector a, NaiveVector b) {
    size_t size = a.size();
    for(int i = 0; i < size; ++i) {
        result[i] = alpha * a[i] + b[i];
    }
}




double VecSummation(NaiveVector a) {
    size_t size = a.size();
    double sum = 0.0;

    for(int i = 0; i < size; ++i) {
        sum += a[i];
    }
    return sum;
}




void VecScale(StdVector & result, double alpha, const StdVector &a) {
    const size_t size = a.size();
    for(size_t i = 0; i < size; ++i) {
        result[i] = alpha * a[i];
    }
}



void VecScaleAdd(StdVector & result, double alpha, const StdVector &a, double beta,const StdVector &b) {
    const size_t size = a.size();
    for(size_t i = 0; i < size; ++i) {
        result[i] = alpha * a[i] + beta*b[i];
    }
}



void swap(BlasVector &other) noexcept complexity_1{
    cblas_dswap(static_cast<int>(size_),other.data_.data(),1,this->data_.data(),1); // todo 1,1
}

void axpy(double alpha, const BlasVector &other) complexity_1 {
    // y = a*x + y
    cblas_daxpy(static_cast<int>(size_), alpha,other.data_.data() , 1, this->data_.data(), 1); // todo 1,1
};

void copy(const BlasVector &other) complexity_1{
    cblas_dcopy(static_cast<int>(size_),other.data_.data(),1,this->data_.data(),1); // todo 1,1
}

void scale(double alpha) complexity_1{
    cblas_dscal(static_cast<int>(size_),alpha,this->data_.data(),1); // todo one
}

[[nodiscard]] double sdot(const BlasVector &other) const complexity_1{
    // todo extend to compensated sum
    return cblas_ddot(static_cast<int>(size_),other.data_.data,1,this->data_.data(),1);
}

[[nodiscard]] double dot(const BlasVector &other) const complexity_1{
    return cblas_ddot(static_cast<int>(size_),other.data_.data,1,this->data_.data(),1);
}

[[nodiscard]] double sum() const complexity_1{
    return std::accumulate(this->data_.begin(),this->data_.end(),0.0);
} // todo

[[nodiscard]] double norm_abs() const complexity_1{
    return cblas_dasum(static_cast<int>(size_),this->data_.data(),1);
}

[[nodiscard]] double norm_l2() const complexity_1{
    return cblas_dnrm2(static_cast<int>(size_),this->data_.data(),1);
}

[[nodiscard]] double norm_max() const complexity_1{
    const size_t index = cblas_idamax(static_cast<int>(size_),this->data_.data(),1);
    return this->data_[index];
}

[[nodiscard]] size_t id_max() const complexity_1{
    return cblas_idamax(static_cast<int>(size_),this->data_.data(),1);
}
