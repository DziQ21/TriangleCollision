#include <iostream>
#include <array>
#include <cmath>

struct vec2
{
    float x, y;
};

struct line
{
    float a, b;
};

struct triangle
{
    std::array<vec2, 3>points;
};

enum class side:bool{down=false,up=true};


bool isColliding(const triangle& triangle1, const triangle& triangle2);
side sideOfLine(const line& line_,const vec2& point);
bool isPointOnSameSide(const line& line,const vec2& point,side side_);
line calculateCoefficients(const vec2& point1,const vec2& point2);
std::ostream& operator<<(std::ostream& os,const triangle& triangle);



int main() {
    //some exampleTriangles

    triangle triangle_1={std::array<vec2, 3>({vec2{0,0},vec2{0,1},vec2{1,0}})};
    triangle triangle_2={std::array<vec2, 3>({vec2{0.5,0},vec2{0.5,1},vec2{1.5,0}})};
    std::cout<<"for triangle:\n"<<triangle_1<<"\nand: \n"<<triangle_2;
    if(isColliding(triangle_1,triangle_2))
        std::cout<<"\ncollision occurs\n";
    else
        std::cout<<"\ncollision doesn't occur\n";

    triangle_1={std::array<vec2, 3>({vec2{0,0},vec2{0,1},vec2{1,0}})};
    triangle_2={std::array<vec2, 3>({vec2{0,0},vec2{0,1},vec2{-1,0}})};
    std::cout<<"for triangle:\n"<<triangle_1<<"\nand: \n"<<triangle_2;
    if(isColliding(triangle_1,triangle_2))
        std::cout<<"\ncollision occurs\n";
    else
        std::cout<<"\ncollision doesn't occur\n";

    triangle_1={std::array<vec2, 3>({vec2{1,2},vec2{2,1},vec2{1,1}})};
    triangle_2={std::array<vec2, 3>({vec2{3,0},vec2{0,0},vec2{0,3}})};
    std::cout<<"for triangle:\n"<<triangle_1<<"\nand: \n"<<triangle_2;
    if(isColliding(triangle_1,triangle_2))//edge case triangle inside triangle
        std::cout<<"\ncollision occurs\n";
    else
        std::cout<<"\ncollision doesn't occur\n";

    triangle_1={std::array<vec2, 3>({vec2{0,0},vec2{0,1},vec2{1,0}})};
    triangle_2={std::array<vec2, 3>({vec2{3,0},vec2{3,1},vec2{2,0}})};
    std::cout<<"for triangle:\n"<<triangle_1<<"\nand: \n"<<triangle_2;
    if(isColliding(triangle_1,triangle_2))//edge case triangle inside triangle
        std::cout<<"\ncollision occurs\n";
    else
        std::cout<<"\ncollision doesn't occur\n";

    std::cout<<"type first triangle coordinates:\n";
    for(int i=0;i<3;i++){
        std::cin>>triangle_1.points[i].x>>triangle_1.points[i].y;
    }
    std::cout<<"type second triangle coordinates:\n";
    for(int i=0;i<3;i++){
        std::cin>>triangle_2.points[i].x>>triangle_2.points[i].y;
    }
    std::cout<<"for triangle:\n"<<triangle_1<<"\nand: \n"<<triangle_2;
    if(isColliding(triangle_1,triangle_2))//edge case triangle inside triangle
        std::cout<<"\ncollision occurs\n";
    else
        std::cout<<"\ncollision doesn't occur\n";
    return 0;
}

//You can prove that the two triangles do not collide by finding an edge
// that acts as a separating line where all the vertices of one triangle lie on one side and
// the vertices of the other triangle lie on the other side.
bool isColliding(const triangle& triangle1, const triangle& triangle2 ){
    for(int i=0;i<3;i++) {
        line coefficients = calculateCoefficients(triangle1.points[i%3], triangle1.points[(i+1)%3]);
        //(i+1)%3 when i==2 it calculates coeficients of line for 0th point and for last point
        side side_ = sideOfLine(coefficients, triangle1.points[(i+2)%3]);

        bool areAllOnOtherSide=true;
        for (auto& point :triangle2.points) {
            if (isPointOnSameSide(coefficients, point, side_)) {
                areAllOnOtherSide= false;
                break;
            }
        }
        if(areAllOnOtherSide)
            return false;
    }
    for(int i=0;i<3;i++) {
        line coefficients = calculateCoefficients(triangle2.points[i], triangle2.points[(i+1)%3]);
        side side_ = sideOfLine(coefficients, triangle2.points[(i+2)%3]);
        bool areAllOnOtherSide=true;
        for (auto& point :triangle1.points) {
            if (isPointOnSameSide(coefficients, point, side_)) {
                areAllOnOtherSide= false;
                break;
            }
        }
        if(areAllOnOtherSide)
            return false;
    }
    return true;
}
line calculateCoefficients(const vec2& point1,const vec2& point2){
    float deltaX=point1.x-point2.x;
    if(deltaX==0)//edge case example: p1=(1,0) 2p=(1,1)
        return {INFINITY,point1.x};
    else {
        float aCoefficient = (point1.y - point2.y) / deltaX;
        return {aCoefficient, point1.y - point1.x * aCoefficient};
    }
}
bool isPointOnSameSide(const line& line_,const vec2& point,side side_){
    if(line_.a!= INFINITY)
        return (side)(line_.a * point.x  + line_.b< point.y) == side_ or line_.a * point.x + point.y - line_.b==0;
    return (side)(point.x>line_.b)==side_ or point.x==line_.b;
}
side sideOfLine(const line& line_,const vec2& point){
    if(line_.a!= INFINITY)
        return line_.a * point.x + line_.b< point.y ? side::up : side::down;
    return point.x>line_.b ? side::up : side::down;
}
std::ostream& operator<<(std::ostream& os,const triangle& triangle){

    for(int i=0;i<3;i++){
        os<<"p"<<i+1<<" = "<<triangle.points[i].x<<" "<<triangle.points[i].y<<"\t";
    }
    return os;
}