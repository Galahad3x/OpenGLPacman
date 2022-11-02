using namespace std;

class Food {
    public:
        bool operator == (const Food& food) const { 
            return x == food.x && y == food.y && size==food.size;
        }
        bool operator != (const Food& food) const { return !operator==(food); }
        float x,y;
        float size;
        Food(float x, float y, float size);
        void draw();
};