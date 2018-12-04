#ifndef MTNET_BASE_UNCOPYABLE_H
#define MTNET_BASE_UNCOPYABLE_H
//Uncopyable 参考Effective C++

namespace mtnet
{
class Uncopyable
{
public:
protected:
	Uncopyable(){};    //允许派生类对象构造和析构
	~Uncopyable(){};
private:
	Uncopyable(const Uncopyable&);    //阻止copying
	Uncopyable& operator=(const Uncopyable&);
};

}

#endif  //MTNET_BASE_UNCOPYABLE_H