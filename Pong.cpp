#include <iostream>
#include <raylib.h>

using namespace std;

int player_score = 0;
int cpu_score = 0;
int winningScore = 2;
bool playerScored = false;
bool cpuScored = false;
bool running = true;

class Ball {
public:
	float x, y;
	int speed_x, speed_y;
	int radius;

	void Draw() {
		DrawCircle(x, y, radius, WHITE);
	}

	void Update() {
		if (y - radius <= 0 || y + radius >= GetScreenHeight()) {
			speed_y *= -1;
		}

		if (x + radius >= GetScreenWidth()) {
			player_score += 1;
			speed_x *= -1;
			x = GetScreenWidth()/2;
			y = GetScreenHeight()/2;
			playerScored = true;
		}

		if (x - radius <= 0) {
			cpu_score += 1;
			speed_x *= -1;
			x = GetScreenWidth() / 2;
			y = GetScreenHeight() / 2;
			cpuScored = true;
		}

		x += speed_x;
		y += speed_y;
	}
};

class Paddle {
public:
	float x, y;
	int speed;
	float width, height;
	Color color;

	void Draw() {
		DrawRectangle(x,y,width,height,color);
	}

	void Update() {
		if (IsKeyDown(KEY_W)) {
			if (!IsKeyDown(KEY_S)) {
				if (y > 0) {
					y -= speed;
				}
			}
		} else if (IsKeyDown(KEY_S)) {
			if (!IsKeyDown(KEY_W)) {
				if (y + height < GetScreenHeight()) {
					y += speed;
				}
			}
		}
	}
};

class CpuPaddle: public Paddle {
public:
	void Update(int ball_y) {
		int center = y + height;
		if (center > ball_y) {
			if (y > 0) {
				y -= speed;
			}
		}

		if (center < ball_y) {
			if (y + height < GetScreenHeight()) {
				y += speed;
			}
		}
	}
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main()
{
	cout << "Starting the game" << endl;
	const int screen_width = 1280;
	const int screen_height = 800;
	InitWindow(screen_width, screen_height, "My Pong Game!");
	SetTargetFPS(60);

	ball.radius = 20;
	ball.x = screen_width / 2;
	ball.y = screen_height / 2;
	ball.speed_x = 7;
	ball.speed_y = 7;

	player.color = WHITE;
	player.width = 25;
	player.height = 120;
	player.x = 10;
	player.y = screen_height/2 - player.height / 2;
	player.speed = 6;

	cpu.color = RED;
	cpu.width = 25;
	cpu.height = 120;
	cpu.x = screen_width - 10 - cpu.width;
	cpu.y = screen_height / 2 - player.height / 2;
	cpu.speed = 6;

	while(WindowShouldClose() == false && running) {
		BeginDrawing();

		if (player_score < winningScore && cpu_score < winningScore) {
			ball.Update();
			player.Update();
			cpu.Update(ball.y);

			if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player.x,player.y,player.width,player.height })) {
				ball.speed_x *= -1;
			}

			if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ cpu.x,cpu.y,cpu.width,cpu.height })) {
				ball.speed_x *= -1;
			}

			if (playerScored || cpuScored) {
				playerScored = false;
				cpuScored = false;
			}

			ClearBackground(BLACK);

			ball.Draw();
			player.Draw();
			cpu.Draw();

			DrawText(TextFormat("%i", player_score), screen_width / 2 - 100, 15, 50, player.color);
			DrawText(TextFormat("%i", cpu_score), screen_width / 2 + 100, 15, 50, cpu.color);
		}
		else {
			ClearBackground(BLACK);
			if (player_score == winningScore) {
				DrawText("YOU WIN!", screen_width / 2, screen_height / 2, 50, player.color);
			}
			else if (cpu_score == winningScore) {
				DrawText("CPU WON!", screen_width / 2, screen_height / 2, 50, cpu.color);
			}

			running = false;
		}

		EndDrawing();
	}

	_Thrd_sleep_for(3000);
	CloseWindow();
	return 0;
}