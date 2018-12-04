#ifndef MTNET_BASE_UNCOPYABLE_H
#define MTNET_BASE_UNCOPYABLE_H
//Uncopyable �ο�Effective C++

namespace mtnet
{
class Uncopyable
{
public:
protected:
	Uncopyable(){};    //��������������������
	~Uncopyable(){};
private:
	Uncopyable(const Uncopyable&);    //��ֹcopying
	Uncopyable& operator=(const Uncopyable&);
};

}

#endif  //MTNET_BASE_UNCOPYABLE_H