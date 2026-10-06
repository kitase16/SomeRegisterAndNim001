#include <iostream>
#include <cstdint>
#include <memory>


//this up as it up. but compiler is nearly so crashing. so near,


struct Byte {
	union {
		struct { std::uint8_t X; };
		union {
			std::int8_t Signed;
			std::uint8_t Unsigned;
		};
	};
};


struct Word {
	union {
		struct { Byte A, B; };
		union {
			std::int16_t Signed;
			std::uint16_t Unsigned;
			Byte Array[2];
		};
	};
};

struct DWord {
	union {
		struct { Word A, B; };
		union {
			std::int32_t Signed;
			std::uint32_t Unsigned;
			Byte Array[4];
		};
	};
};


struct QWord {
	union {
		struct { DWord A, B; };
		union {
			std::int64_t Signed;
			std::uint64_t Unsigned;
			Byte Array[8];
		};
	};

};

template<class T>
class Sign {//フラグ在りナンバー計算体。
public:
	Sign(T& value) : V(&value) = default;
	Sign(std::shared_ptr<T> value) : SharedPtr(value), V(SharedPtr.get()) {}
	struct Byte {
		union {
			struct { std::uint8_t X; };
			union {
				std::int8_t Signed;
				std::uint8_t Unsigned;
			};
		};
	};


	struct Word {
		union {
			struct { Byte A, B; };
			union {
				std::int16_t Signed;
				std::uint16_t Unsigned;
				Byte Array[2];
			};
		};
	};

	template<class U>
	Sign<T> operator+(const Sign<U>& other) const {

		Sign<T> R{ std::shared_ptr<T>(); };

		R.V->Unsigned = 0;

		for (std::size_t i = 0; i < sizeof(R.V->Array)-1; ++i) {
			Word* WA = &V->Array[i];
			Word* WB = &other.V->Array[i];
			Word* WR = &R.V->Array[i];

			*WR = *WA + *WB+*WR;
		}
		return R;
	}

	template<class U>
	Sign<T> operator-(const Sign<U>& other) const {

		Sign<T> R{ std::shared_ptr<T>(); };

		R.V->Unsigned = 0;

		int L = sizeof(R.V->Array) - 1;

		for (std::size_t i = 0; i < L; ++i) {
			Word* WA = &V->Array[i];
			Word* WB = &other.V->Array[i];
			Word* WR = &R.V->Array[i];

			WR->Unsigned = WA->Unsigned - WB->Unsigned;

			if (WR->Singed < 0) {
				i--;
			}

			for (std::size_t j = i; j < L; ++j) {
				R.V->Array[j].Unsigned = V->Array[j].Unsigned - other.V->Array[j].Unsigned;
				if (R.V->Array[j].Signed < 0) {
					V->Array[j+1].Unsigned--;

				}
				else {
					break;
				}
			}
			
			WR->Unsigned = WA->Unsigned - WB->Unsigned;
		}
		return R;
	}

	template<class U>
	Sign<T> operator*(const Sign<U>& other) const {
		Sign<T> R{ std::shared_ptr<T>() };

		for (std::size_t i = 0; i < other; i++) {
			R = R + *this;
		}

		return R;

	}

	Sign<T> operator()() const {
		Sign<T> R{ std::shared_ptr<T>() };

		R->A = this->A;
		R->B = this->B;;

		return R;
	}

	template<class U>
	Sign<T> operator/(const Sign<U>& other) const {
		Sign<T> R{ std::shared_ptr<T>() };

		R= *this;

		Sing<U> O{std::shared_ptr<U>()};

		for (O = 0; O < other; O = O + 1) {
			R = R -(other*O);

			if (R < 0) {
				break;
			}
		}

		O = O - 1;;
		return O;
	}
	template<class U>
	Sign<T> operator%(const Sign<U>& other) const {
		Sign<T>	R{ std::shared_ptr<T>() };

		Sign<U> O{ std::shared_ptr<U>() };

		for (O = 0; O < other; O = O + 1) {
			R = R - (other * O);
			if (R < 0) {
				break;
			}
		}
		R = R - (other * (O - 1));
		return R;
	}

	template<class U>
	Sign<T> operator =(const Sign<U>& other) {
		this->V.A = other.V.A;
		this->V.B = other.V.B;

		return *this;
	}

	template<class U>
	bool operator==(const Sign<U>& other) const {
		for (std::size_t i = 0; i < sizeof(V->Array); ++i) {
			if (V->Array[i].Unsigned != other.V->Array[i].Unsigned) {
				return false;
			}
		}
		return true;
	}
	template<class U>
	bool operator!=(const Sign<U>& other) const {
		return !(*this == other);
	}
	template<class U>
	bool operator<(const Sign<U>& other) const {
		for (std::size_t i = sizeof(V->Array); i > 0; --i) {
			if (V->Array[i - 1].Unsigned < other.V->Array[i - 1].Unsigned) {
				return true;
			}
			else if (V->Array[i - 1].Unsigned > other.V->Array[i - 1].Unsigned) {
				return false;
			}
		}
		return false;
	}
	template<class U>	
	bool operator>(const Sign<U>& other) const {
		return !(*this < other) && !(*this == other);
	}
protected:
	T* V = nullptr;
	std::shared_ptr<T> SharedPtr = nullptr;
};


struct LWord {
	
	struct {
		union {
			struct { QWord A, B; };
			struct { Byte Array[16]; };

		};
		Sign<LWord> Signed{ *this };
		Sign<LWord> Unsigned{ *this };//not normaly		
	};


};

int main() {
	LWord lword;
	lword.Unsigned.V->A.Unsigned = 0x123456789ABCDEF0;
	lword.Unsigned.V->B.Unsigned = 0x0FEDCBA987654321;
	std::cout << "LWord Unsigned A: " << std::hex << lword.Unsigned.V->A.Unsigned << std::endl;
	std::cout << "LWord Unsigned B: " << std::hex << lword.Unsigned.V->B.Unsigned << std::endl;
	return 0;
}


/** /

int main() {
	QWord qword;
	qword.Unsigned = 0x123456789ABCDEF0;
	std::cout << "QWord Unsigned: " << std::hex << qword.Unsigned << std::endl;
	std::cout << "DWord A Unsigned: " << std::hex << qword.A.Unsigned << std::endl;
	std::cout << "DWord B Unsigned: " << std::hex << qword.B.Unsigned << std::endl;
	std::cout << "Word A Unsigned: " << std::hex << qword.A.A.Unsigned << std::endl;
	std::cout << "Word B Unsigned: " << std::hex << qword.A.B.Unsigned << std::endl;
	std::cout << "Byte A Unsigned: " << std::hex << static_cast<int>(qword.A.A.A.Unsigned) << std::endl;
	std::cout << "Byte B Unsigned: " << std::hex << static_cast<int>(qword.A.A.B.Unsigned) << std::endl;
	return 0;
}
/**/