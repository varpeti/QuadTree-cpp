# Quadtree
A quadtree libary written in cpp

## Usage

Include:
```cpp
#include Quadtree.h
```

Initialize:
```cpp
template <typename N, typename V>
N QuadTree<N, V>::minSize = 10*10; // Minimum region size
```

New QuadTree:
```cpp
QuadTree<Number,Value> qt(N x, N y, N w, N h, V value);
```

Set value of a region:
```cpp
qt.setValue(N x, N y, N w, N h, V value);
```

Draw:
```cpp
void toString(N x, N y, N h, N w, N value)
{
    std::cout << x <<" "<< y <<" "<< h <<" "<< w <<" "<< value << "\n";
}

int main()
{
    /* code */
    qt.draw(&toString);
    /* code */
    return 0;
}
```

Draw from context:
```cpp
void forwarder(void* context, N x, N y, N w, N h, MyValue value) 
{
    static_cast<MyClass*>(context)->drawQuadtree(x, y, w, h, value);
}

void MyClass::drawQuadtree(N& x, N& y, N& w, N& h, MyValue& value)
{
    
    if (value==MyValue::air)
    {
        setColor(0,128,255);
    }
    else if (value==MyValue::dirt)
    {
        setColor(0,110,0);
    }
    /* ... */
    drawRectangle(x,y,w,h);
}

void MyClass::draw()
{
    /* code */
    qt.draw(this,&forwarder);
    /* code */
}
```

## Example

test.cpp

## Licens
MIT

## Screenshots
![P1](https://github.com/varpeti/Quadtree/blob/master/quadtree1.png)
![P2](https://github.com/varpeti/Quadtree/blob/master/quadtree2.png)
