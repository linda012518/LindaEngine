#include "UUID.h"

#include <random>
#include <sstream>
#include <iomanip>

std::string LindaEngine::UUID::Get()
{
    {
        std::random_device rd;

        //size_t num=rd();生成一个机器随机数
        std::mt19937_64 gernator(rd());//通过梅森旋转算法，生成一个伪随机数

        //我们要生成的是8个0~255的数
        std::uniform_int_distribution<int> distribution(0, 255);
        //std::cout<<distribution(gernator)<<std::endl;

        //将生成的数字转成16进制
        std::stringstream ss;
        for (int i = 0; i < 8; i++)
        {
            ss << std::setw(2) << std::setfill('0') << std::hex << distribution(gernator);
            if (i == 3 || i == 5 || i == 7)
            {
                ss << "-";
            }
        }
        //定义一个原子类型整数，初始化为1
        static std::atomic<size_t> seq(1);
        size_t num = seq.fetch_add(1);
        for (int i = 7; i >= 0; i--)
        {
            ss << std::setw(2) << std::setfill('0') << std::hex << ((num >> (i * 8)) & 0xff);
            if (i == 6) ss << "-";
        }
        return ss.str();
    }

}
