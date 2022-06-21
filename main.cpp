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
 * ������ġ�� �������� �Լ�
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
 * ������ġ �������� �Լ�
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
	 * ������ ������ ��ġ�� ���� ������
	 * �������� �Դ� ��ΰ� ����Ǿ� �ִ� ť
	 */
	queue<PointNode> PosNodeQueue;

	/**
	 * ���� ���
	 */
	PointNode CurNode;

	Point StartPos; //���� ��ġ
	Point GoalPos; //���� ��ġ
	Point CurPos; //���� ��ġ
	Point PrevPos; //���� ��ġ

	/**
	 * ������ �� �ִ� ����߿� ���� �ڽ�Ʈ�� ���� ��Ʈ
	 */
	PointNode LowCostNode;

	/**
	 * ���������� ������ ����
	 */
	const Point Dir[] = {
			{ 0,-1 },
			{-1, 0 },
			{ 0, 1 },
			{ 1, 0 }
	};

	// ���� ��ġ
	StartPos = GetStartPosition();

	// ���� ��ġ
	GoalPos = GetGoalPosition();

	printf("[ Search (%d,%d) to (%d,%d) ] \n\n", StartPos.X, StartPos.Y, GoalPos.X, GoalPos.Y);

	// ���� ��ġ�� �̵��ߴٴ� ǥ��
	MAP[StartPos.Y][StartPos.X] = 1; // ����ġ 1

	// ���� ��ġ�� �̵��� �� �ִ� �ڸ��� ǥ��
	MAP[GoalPos.Y][GoalPos.X] = 0;

	/**
	 * �ʱ� ���� ���� ������ ��ġ
	 */
	CurNode.CurPos = StartPos;
	CurPos = CurNode.CurPos;

	while (true)
	{

		/* ���� */
		if (CurPos.Y == GoalPos.Y && CurPos.X == GoalPos.X)
		{
			/**
			 * �̵���ΰ� ��������� ó�� ã����
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
		 * ������ ���鼭 ���̾ƴ� ��� ��θ� Ȯ��
		 */
		for (int I = 0; I < 4; I++)
		{
			// ���� ��ġ
			Point NextPos = {
				CurPos.Y + Dir[I].Y,
				CurPos.X + Dir[I].X,
			};

			// ������ Ȯ��
			if (MAP[NextPos.Y][NextPos.X] == -1)
				continue;

			// ������ġ�� ��忡 �ְ� ť�� ����
			CurNode.CurPos = NextPos;
			CurNode.PrevPosStack.push(CurPos);
			PosNodeQueue.push(CurNode);
		}

		while (true)
		{
			// ���� �տ��ִ� ��带 ������
			CurNode = PosNodeQueue.front();
			PosNodeQueue.pop();

			/**
			 * ���� ����ġ�� �̿��Ͽ� ������ �� ��ġ�� ����ġ�� Ȯ�� �ϰ�
			 * ����ġ�� �� ���� �̵��� �� �������� �����ϵ�����
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
			 * ť���� ���� ��带 ���̻� ���� �� ������ ������ �Ұ��� �ϹǷ�
			 * ���α׷��� �����ؾ���
			 */
			if (PosNodeQueue.empty())
			{
				/*
				 * LowCostNode ������� ������ ��������� �ѹ��� ����������
				 * ���ٴ� �ǹ�
				 */
				if (LowCostNode.PrevPosStack.empty())
					cout << "Not Found" << endl;
				else
					PrintLowCostPath(LowCostNode); // ��� ǥ��

				return 0;
			}
		}
	}
}