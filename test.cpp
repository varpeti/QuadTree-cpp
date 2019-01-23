#include <iostream>

#include "QuadTree.h"
template <typename N, typename V>
N QuadTree<N, V>::minSize = 10*10;

void toString(int x, int y, int h, int w, int value)
{
    std::cout << x <<" "<< y <<" "<< h <<" "<< w <<" "<< value << "\n";
}

int main()
{   
    std::cout << "qt(0,0,100,100,0)" << "\n";
    QuadTree<int,int> qt(0,0,100,100,0);

    std::cout << "getValue(15,15) " << qt.getValue(15,15) << "\n";
    std::cout << "draw:" << "\n"; qt.draw(&toString); std::cout << "\n";

    std::cout << "setValue(10,10,10,10,1)" << "\n";
    qt.setValue(10,10,10,10,1); 

    std::cout << "getValue(15,15) " << qt.getValue(15,15) << "\n";
    std::cout << "draw:" << "\n"; qt.draw(&toString); std::cout << "\n";

    std::cout << "setValue(10,10,10,10,0)" << "\n";
    qt.setValue(10,10,10,10,0);
    
    std::cout << "getValue(15,15) " << qt.getValue(15,15) << "\n";
    std::cout << "draw:" << "\n"; qt.draw(&toString); std::cout << "\n";


    getchar();
    return 0;
}