namespace CLAM {

/** Auxiliary class to define the order of a moment at compile time*/
template<int order> struct O
{
	public: static O<order> instance;
};

template<int order> O<order>  O<order>::instance;

//A nicer way of referring to the first orders

extern O<1>* FirstOrder;
extern O<2>* SecondOrder;
extern O<3>* ThirdOrder;
extern O<4>* FourthOrder;
extern O<5>* FifthOrder;

} // namespace CLAM


