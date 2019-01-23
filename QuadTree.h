/*
The MIT License (MIT)

Copyright (c) 2019 Varaljai Peter

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

//#include <iostream>

// N szám típusú (N+N, N-N, N*N, N/2, N=0), V is anything
template <typename N, typename V>
class QuadTree
{
    private:
        N _x,_y;
        N _w,_h;
        V _value;
        bool _leaf;
        N _area;

    public:
        QuadTree<N, V> *ur;
        QuadTree<N, V> *dr;
        QuadTree<N, V> *dl;
        QuadTree<N, V> *ul;

        static N minSize;

    public:

        QuadTree(N x, N y, N w, N h, V value) :
            _x(x), _y(y), _w(w), _h(h), _value(value), _leaf(true), _area(w*h)
        {
            ur=nullptr;
            dr=nullptr;
            dl=nullptr;
            ul=nullptr;
        }

        QuadTree(const QuadTree *ori)
        {
            _x      = ori->_x;
            _y      = ori->_y;
            _w      = ori->_w;
            _h      = ori->_h;
            _value  = ori->_value;
            _leaf   = ori->_leaf;
            _area   = _w*_h;
            ur      = nullptr;
            dr      = nullptr;
            dl      = nullptr;
            ul      = nullptr;

            if (_leaf) return;

            ur = new QuadTree(ori->ur);
            dr = new QuadTree(ori->dr);
            dl = new QuadTree(ori->dl);
            ul = new QuadTree(ori->ul);
        }

        ~QuadTree()
        {
            if (!_leaf)
            {
                delete ur;
                delete dr;
                delete dl;
                delete ul;
            }
        }

        // módosítja egy tartomány típúsát a fában
        void setValue(N x, N y, N w, N h, V value)
        {   
            // ha teljesen kitölti a cellát (és/vagy túl is nyúl rajta)
            if (isFullyContained(x,y,w,h)) 
            {
                //std::cout << "isFullyContained" << "\n";
                if (!_leaf) merge();
                _value = value;
                _leaf = true;
                return;
            }
            // ha teljesen kívül van
            else if (isFullyOutside(x,y,w,h))
            {
                //std::cout << "isFullyOutside" << "\n";
                return;
            }
            // Ha benne van egy cellában de nem tölti ki teljesen
            else
            {
                //std::cout << "else" << "\n";
                // ha túl kicsi a test, nem rajzolja be
                if (_area <= minSize) return;

                //Ha levél a cella akkor felosztja 4 részre (Child)
                if (_leaf) 
                {
                    //std::cout << "split" << "\n";
                    split();
                }

                //std::cout << "setChildValues" << "\n";
                setChildValues(x,y,w,h,value);
            }
        }

        V getValue(N x, N y)
        {
            V value = V();
            getValue(x, y, value);
            return value;
        }

        void getValue(N x, N y, V &value)
        {
            if (isFullyOutside(x,y,0,0)) return;

            if (_leaf)
            {
                value = _value;
                return;
            }
            
            ur->getValue(x, y, value);
            dr->getValue(x, y, value);
            dl->getValue(x, y, value);
            ul->getValue(x, y, value);
        }

        void draw(void (*fptr)(N, N, N, N, V) )
        {
            if (_leaf)
            {
                fptr(_x, _y, _w, _h, _value);
            }
            else
            {
                ur->draw(fptr);
                dr->draw(fptr);
                dl->draw(fptr);
                ul->draw(fptr);
            }
        }

        void draw(void *context, void (*fptr)(void*,N, N, N, N, V))
        {
            if (_leaf)
            {
                fptr(context,_x, _y, _w, _h, _value);
            }
            else
            {
                ur->draw(context,fptr);
                dr->draw(context,fptr);
                dl->draw(context,fptr);
                ul->draw(context,fptr);
            }
        }

    private:

        // true ha a test teljesen kitölti (és/vagy túl is nyúl rajta)
        bool isFullyContained(N x, N y, N w, N h)
        {
            return (    x <= _x      &&
                    x + w >= _x + _w &&
                        y <= _y      &&
                    y + h >= _y + _h);
        }

        // true ha a testnek és a cellának nincsen egy közös pontja se
        bool isFullyOutside(N x, N y, N w, N h)
        {
            return (_x + _w <= x  ||
                    _y + _h <= y  ||
                     x + w  <= _x ||
                     y + h  <= _y);
        }

        // Megváltoztatja a 4 (Child) cella típusát
        void setChildValues(N x, N y, N w, N h, V value)
        {
            ur->setValue(x,y,w,h,value);
            dr->setValue(x,y,w,h,value);
            dl->setValue(x,y,w,h,value);
            ul->setValue(x,y,w,h,value);

            // Megvizsgálja hogy mind a 4-nek ugyan olyan-e a típusas
            bool same = true;
            if (!ur->_leaf || !dr->_leaf || !dl->_leaf || !ul->_leaf)
            {
                same=false;
            }
            else
            {
                if (ur->_value != dr->_value) 
                {
                    same=false;
                }
                else if (ur->_value != dl->_value)
                {
                    same=false;
                }
                else if (ur->_value != ul->_value)
                {
                    same=false;
                }
            }

            // Ha igen, összevonja őket.
            if (same)
            {
                merge();
            }
        }

        // Felosztja 4 részre a cellát
        void split()
        {
            _leaf = false;
            N cw = _w/2;
            N ch = _h/2;

            ur = new QuadTree(_x,    _y,    cw, ch, _value);
            dr = new QuadTree(_x+cw, _y,    cw, ch, _value);
            dl = new QuadTree(_x,    _y+ch, cw, ch, _value);
            ul = new QuadTree(_x+cw, _y+ch, cw, ch, _value);
        }

        // Összevon egy szülőhöz tartozó 4 cellát, és levél lesz
        void merge()
        {
            _leaf=true;
            _value = ur->_value;
            delete ur;
            delete dr;
            delete dl;
            delete ul;
            // Ha nem levelek, a destruktor meghívja a gyerekeire.
        }
    
};