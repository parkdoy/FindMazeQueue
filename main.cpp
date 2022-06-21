#include <iostream>
#include <queue>
#include <stack>
#include <conio.h>

using namespace std;

#define WEIGHT 1

int MAP[10][10] =
{
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1,  'p',  0,  0,  0,  0,  0, -1, -1, -1},
	{-1,  0, -1, -1, -1, -1,  0, 0, -1, -1},
	{-1,  0,  0,  0,   0,  0,  0, -1, 0, -1},
	{-1,  0, -1, -1,  0, -1, -1, -1, 0, -1},
	{-1,  0,  0,  0,   0,  0,  0,  -1, 0, -1},
	{-1,  0, -1, -1, -1, -1,  0, -1, 'g', -1},
	{-1,  0,  0,  0,  -1, -1, -1, -1, 0, -1},
	{-1,  0, -1,  0,   0,  0,  0,  0,  0, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
};

class Point
{
public:
	int Y;
	int X;
};

class PointNode
{
public:
	Point CurPos;
	stack<Point> PrevPosStack;
};

/*
 * 시작위치를 가져오는 함수
 */
Point GetStartPosition()
{
	for (int I = 0; I < 10; I++)
	{
		for (int J = 0; J < 10; J++)
		{
			if (MAP[I][J] == 'p')
			{
				return { I, J };
			}
		}
	}
	return { -1,-1 };
}

/**
 * 도착위치 가져오는 함수
 */
Point GetGoalPosition()
{
	for (int I = 0; I < 10; I++)
	{
		for (int J = 0; J < 10; J++)
		{
			if (MAP[I][J] == 'g')
			{
				return { I, J };
			}
		}
	}
	return { -1,-1 }; 
}

void PrintLowCostPath(PointNode& Node)
{
	for (int Y = 0; Y < 10; ++Y)
	{
		for (int X = 0; X < 10; ++X)
		{
			if (MAP[Y][X] == -1)
				MAP[Y][X] = '*';
			else
				MAP[Y][X] = ' ';
		}
	}

	MAP[Node.CurPos.Y][Node.CurPos.X] = 'o';
	while (!Node.PrevPosStack.empty())
	{
		Point Pos = Node.PrevPosStack.top();
		Node.PrevPosStack.pop();
		MAP[Pos.Y][Pos.X] = 'o';
	}

	for (int Y = 0; Y < 10; ++Y)
	{
		for (int X = 0; X < 10; ++X)
		{
			cout << (char)MAP[Y][X];
		}
		cout << endl;
	}
}

int main()
{
	/**
	 * 앞으로 가야할 위치에 대한 정보와
	 * 이전까지 왔던 경로가 저장되어 있는 큐
	 */
	queue<PointNode> PosNodeQueue;

	/**
	 * 현재 노드
	 */
	PointNode CurNode;

	Point StartPos; //시작 위치
	Point GoalPos; //골인 위치
	Point CurPos; //현재 위치
	Point PrevPos; //이전 위치

	/**
	 * 도착할 수 있는 노드중에 가장 코스트가 적은 루트
	 */
	PointNode LowCostNode;

	/**
	 * 루프문으로 방향을 지정
	 */
	const Point Dir[] = {
			{ 0,-1 },
			{-1, 0 },
			{ 0, 1 },
			{ 1, 0 }
	};

	// 시작 위치
	StartPos = GetStartPosition();

	// 도착 위치
	GoalPos = GetGoalPosition();

	printf("[ Search (%d,%d) to (%d,%d) ] \n\n", StartPos.X, StartPos.Y, GoalPos.X, GoalPos.Y);

	// 시작 위치에 이동했다는 표시
	MAP[StartPos.Y][StartPos.X] = 1; // 가중치 1

	// 도착 위치를 이동할 수 있는 자리로 표시
	MAP[GoalPos.Y][GoalPos.X] = 0;

	/**
	 * 초기 노드는 현재 시작할 위치
	 */
	CurNode.CurPos = StartPos;
	CurPos = CurNode.CurPos;

	while (true)
	{

		/* 도착 */
		if (CurPos.Y == GoalPos.Y && CurPos.X == GoalPos.X)
		{
			/**
			 * 이동경로가 비어있으면 처음 찾은것
			 */
			if (LowCostNode.PrevPosStack.empty())
			{
				LowCostNode = CurNode;
			}
			else
			{
				int CurWeight = MAP[CurPos.Y][CurPos.X];
				int GoalWeight = MAP[LowCostNode.CurPos.Y][LowCostNode.CurPos.X];
				if (CurWeight < GoalWeight)
					LowCostNode = CurNode;
			}
		}

		/**
		 * 루프를 돌면서 벽이아닌 모든 경로를 확인
		 */
		for (int I = 0; I < 4; I++)
		{
			// 다음 위치
			Point NextPos = {
				CurPos.Y + Dir[I].Y,
				CurPos.X + Dir[I].X,
			};

			// 벽인지 확인
			if (MAP[NextPos.Y][NextPos.X] == -1)
				continue;

			// 다음위치를 노드에 넣고 큐에 삽입
			CurNode.CurPos = NextPos;
			CurNode.PrevPosStack.push(CurPos);
			PosNodeQueue.push(CurNode);
		}

		while (true)
		{
			// 가장 앞에있는 노드를 꺼내기
			CurNode = PosNodeQueue.front();
			PosNodeQueue.pop();

			/**
			 * 지금 가중치를 이용하여 다음에 갈 위치의 가중치를 확인 하고
			 * 가중치를 더 낮게 이동할 수 있을때만 진행하도록함
			 */
			CurPos = CurNode.CurPos;
			PrevPos = CurNode.PrevPosStack.top();

			if (MAP[CurPos.Y][CurPos.X] == 0 ||
				MAP[CurPos.Y][CurPos.X] > MAP[PrevPos.Y][PrevPos.X] + WEIGHT)
			{
				MAP[CurPos.Y][CurPos.X] = MAP[PrevPos.Y][PrevPos.X] + WEIGHT;
				break;
			}

			/**
			 * 큐에서 다음 노드를 더이상 꺼낼 수 없으면 진행이 불가능 하므로
			 * 프로그램을 종료해야함
			 */
			if (PosNodeQueue.empty())
			{
				/*
				 * LowCostNode 이전경로 스택이 비어있으면 한번도 도착한적이
				 * 없다는 의미
				 */
				if (LowCostNode.PrevPosStack.empty())
					cout << "Not Found" << endl;
				else
					PrintLowCostPath(LowCostNode); // 결과 표시

				return 0;
			}
		}
	}
}