#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <string.h>

using namespace std;

const int N = 9;
const double INF = 1234;

// Point class for convenience, for calculating square vertices.
struct Point {
    Point() {}
    Point(double x, double y) :
      x(x), y(y) {}
    Point operator+(const Point& other) const {
        Point res;
        res.x = this->x + other.x;
        res.y = this->y + other.y;
        return res;
    }
    Point operator-(const Point& other) const {
        Point res;
        res.x = this->x - other.x;
        res.y = this->y - other.y;
        return res;
    }
    Point operator/(int divisor) const {
        Point res;
        res.x = this->x / divisor;
        res.y = this->y / divisor;
        return res;
    }
    bool operator<(const Point& other) const {
        if (this->x == other.x) {
            return (this->y < other.y);
        } else {
            return (this->x < other.x);
        }
    }
    double x,y;
};

inline bool isPointOnGrid(const Point& p) {
    return (p.x >= 0 && p.x < N && p.y >=0 && p.y < N);
}

bool setContains(const Point& p, const set<Point>& s) {
  return (s.find(p) != s.end());
}

bool isPointOnGridAndInSet(const Point& p, const set<Point>& s) {
  return isPointOnGrid(p) && setContains(p,s);
}

// Determines scores for points on the grid based on possible squares.
// Scores are chosen by instinct, and this should be re-evaluated.
// Points that would make a square get -square_penalty score.
// Points that would make the third vertex of the square get -1, unless the fourth vertex is outside the grid. In that case it gets +1.
void processVertices(const set<Point>& mine,
                     const set<Point>& enemy,
                     const set<Point>& free,
                     const Point& third, const Point& fourth,
                     int (&places)[N][N],
                     int square_penalty) {
    int tx = third.x;
    int ty = third.y;
    int fx = fourth.x;
    int fy = fourth.y;

    if (isPointOnGridAndInSet(third, free) && !isPointOnGrid(fourth)) {
        places[tx][ty]++;
    } else if (isPointOnGridAndInSet(fourth, free) && !isPointOnGrid(third)) {
        places[fx][fy]++;
    } else if (isPointOnGrid(third) && isPointOnGrid(fourth)) {
        if (setContains(third, free)) {
            if (setContains(fourth, free)) {
                places[tx][ty]--;
                places[fx][fy]--;
            } else if (setContains(fourth, enemy)) {
                places[tx][ty]++;
            } else if (setContains(fourth, mine)) {
                places[tx][ty]=-square_penalty;
            }
        } else if (setContains(fourth, free)) {
            if (setContains(third, enemy)) {
                places[fx][fy]++;
            } else if (setContains(third, mine)) {
                places[fx][fy]=-square_penalty;
            }
        }
    }

}

// Goes through all pairs of points for both the player and the opponent.
// Sends the result to processVertices for determining penalties of each free location on the grid.
void findPossibleSquares(const set<Point>& dots,
                         const set<Point>& enemy,
                         const set<Point>& free,
                         int (&places)[N][N],
                         int square_penalty) {
    for (auto i = dots.begin(); i!=dots.end(); ++i) {
        for (auto j = i; ++j!=dots.end(); ) {
            Point third, fourth;
            // check if it could be a diagonal
            Point center = (*i + *j) / 2;
            Point half = (*i - *j) / 2;
            third.x = center.x - half.y;
            third.y = center.y + half.x;
            fourth.x = center.x + half.y;
            fourth.y = center.y - half.x;

            if (third.x == floor(third.x) &&
                third.y == floor(third.y) &&
                fourth.x == floor(fourth.x) &&
                fourth.y == floor(fourth.y)) {
                processVertices(dots, enemy, free, third, fourth, places, square_penalty);
            }

            // process other two options for sides
            Point side = *i - *j;
            swap(side.x, side.y);
            side.x = -side.x;
            third = *i + side;
            fourth = *j + side;
            processVertices(dots, enemy, free, third, fourth, places, square_penalty);
            third = *i - side;
            fourth = *j - side;
            processVertices(dots, enemy, free, third, fourth, places, square_penalty);
        }
    }
}

void nextMove(char player, vector <string> board){
    // Process the board and find r and b dots
    char opponent = (player == 'r' ? 'b' : 'r');
    set<Point> mine, enemy, free;
    
    // Scores for each location, evaluated the same way for player and opponent.
    int my_places[N][N];
    int enemy_places[N][N];
    memset(my_places, 0, sizeof(my_places[0][0])*N*N);
    memset(enemy_places, 0, sizeof(enemy_places[0][0])*N*N);
    
    // Read the board.
    for (int i=0; i<board.size(); ++i) {
        for (int j=0; j<board[i].size(); ++j) {
            if (board[i][j] == player) {
                mine.insert(Point(i,j));
                my_places[i][j] -= INF;
            } else if (board[i][j] == opponent) {
                enemy.insert(Point(i,j));
                my_places[i][j] -= INF;
            } else {
                free.insert(Point(i,j));
            }
        }
    }

    // Process possible squares the same way for player and opponent.
    findPossibleSquares(mine, enemy, free, my_places, INF);
    findPossibleSquares(enemy, mine, free, enemy_places, 100);

    // Score each free location on the board.
    // Takes into account previously calculated scores for both player and opponent.
    // Also favours points that are further from the center, that way making possible more squares with 3 vertices inside and 1 outside the board.
    double best = -INF*INF;
    Point best_point(-1,-1);
    for (const auto it : free) {
        if (my_places[static_cast<int>(it.x)][static_cast<int>(it.y)] +
            1*enemy_places[static_cast<int>(it.x)][static_cast<int>(it.y)] +
            abs(4-it.x)+abs(4-it.y) >= best) {
            best = my_places[static_cast<int>(it.x)][static_cast<int>(it.y)] +
                   1*enemy_places[static_cast<int>(it.x)][static_cast<int>(it.y)] +
                   abs(4-it.x)+abs(4-it.y);
            best_point = it;
        }
    }

    cout << best_point.x << " " << best_point.y << endl;
}


int main(void) {

    char player;
    vector <string> board;

    cin >> player;

    for(int i=0; i<9; i++) {
        string s; cin >> s;
        board.push_back(s);
    }

    nextMove(player,board);

    return 0;
}
