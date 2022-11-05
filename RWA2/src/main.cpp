#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include "../include/mouse/mouse.h"
#include "../include/api/api.h"

int main()
{
	// Initializing variables
	rwa2::Node n;
	rwa2::Node goal;
	rwa2::Mouse mouse;
	std::stack<rwa2::Node> s;
	std::stack<rwa2::Node> stk_inv;
	std::vector<rwa2::Node> v;
	std::vector<rwa2::Node> v1;
	std::vector<rwa2::Node> bk_trk;
	goal.set_x(8);
	goal.set_y(8);
	mouse.display_walls();
	bool goal_found;

	// loops for mouse to reach the target position
	while (true)
	{
		// setting colors and text for simulator
		API::clearAllColor();
		API::setColor(0, 0, 'g');
		API::setColor(8, 8, 'r');
		API::setText(0, 0, "Start");
		API::setText(8, 8, "Goal");
		n = *mouse.get_maze(mouse.get_mx(), mouse.get_my());
		while (!bk_trk.empty())
		{
			bk_trk.pop_back();
		}
		while (!stk_inv.empty())
		{
			stk_inv.pop();
		}

		// to search the maze in mouse.cpp 
		goal_found = mouse.search_maze(n, goal, s, bk_trk, v);
		if (!goal_found || n == goal)
			break;
		int stk_len2 = s.size();
		int stk_len3 = 0;
		while (!v.empty())
		{
			v.pop_back();
		}

		// Updating the Visited nodes vector
		for (int i = 0; i < v1.size(); i++)
		{
			v.push_back(v1.at(i));
		}
		for (int i = 0; i < bk_trk.size(); i++)
		{
			if (mouse.is_in_vector(bk_trk.at(i), v1))
				stk_len3++;
		}

		// To update the back track nodes for the backtracking of mouse
		for (int i = 0; i < stk_len2; i++)
		{
			API::setColor(s.top().get_x(), s.top().get_y(), 'w');
			API::setColor(goal.get_x(), goal.get_y(), 'r');
			if (mouse.is_in_vector(s.top(), v1))
			{
				stk_inv.push(s.top());
				s.pop();
				break;
			}
			stk_inv.push(s.top());
			s.pop();
		}
		for (int i = bk_trk.size() - 1; i >= 0; i--)
		{
			if (mouse.is_in_vector(bk_trk.at(i), v1))
			{
				API::setColor(bk_trk.at(i).get_x(), bk_trk.at(i).get_y(), 'w');
				stk_inv.push(bk_trk.at(i));
			}
		}
		if (stk_inv.top() == *mouse.get_maze(mouse.get_mx(), mouse.get_my()))
		{
			if (stk_len3 == 0)
			{
				s.push(stk_inv.top());
			}
			stk_inv.pop();
			if (!stk_len3 == 0)
				stk_len3--;
		}
		stk_len2 = stk_inv.size();

		// To check the wall in the front and update the maze
		for (int i = 0; i < stk_len2; i++)
		{

			// Moves the mouse in north direction
			if (stk_inv.top().get_y() == mouse.get_my() + 1)
			{
				if (mouse.get_mdirection() == NORTH)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
						break;
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
						std::cerr << "Moving Forward!!!!"
								  << "\n";
					}
				}
				else if (mouse.get_mdirection() == EAST)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
						break;
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					mouse.turn_left();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
				else if (mouse.get_mdirection() == SOUTH)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					std::cerr << "Going Back!!!!"
							  << "\n";
					mouse.turn_right();
					mouse.turn_right();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
				else
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
						break;
					}
					std::cerr << "Turning Right!!!!"
							  << "\n";
					mouse.turn_right();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
			}

			// Moves the mouse in east direction
			else if (stk_inv.top().get_x() == mouse.get_mx() + 1)
			{
				if (mouse.get_mdirection() == EAST)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
						break;
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					std::cerr << "Moving Forward!!!!"
							  << "\n";
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
				else if (mouse.get_mdirection() == SOUTH)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
						break;
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					std::cerr << "Moving Left!!!!"
							  << "\n";
					mouse.turn_left();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
				else if (mouse.get_mdirection() == WEST)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					std::cerr << "Going Backwards!!!!"
							  << "\n";
					mouse.turn_right();
					mouse.turn_right();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
				else
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
						break;
					}
					std::cerr << "Turning Right!!!!"
							  << "\n";
					mouse.turn_right();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
			}

			// Moves the mouse in south direction
			else if (stk_inv.top().get_y() == mouse.get_my() - 1)
			{
				if (mouse.get_mdirection() == SOUTH)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
						break;
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					std::cerr << "Moving Forward!!!!"
							  << "\n";
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
				else if (mouse.get_mdirection() == WEST)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
						break;
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					std::cerr << "Moving Left!!!!"
							  << "\n";
					mouse.turn_left();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
				else if (mouse.get_mdirection() == NORTH)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					std::cerr << "Moving Backwards!!!!"
							  << "\n";
					mouse.turn_right();
					mouse.turn_right();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
				else
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
						break;
					}
					std::cerr << "Turning Right!!!!"
							  << "\n";
					mouse.turn_right();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
			}

			// Moves the mouse in west direction
			else if (stk_inv.top().get_x() == mouse.get_mx() - 1)
			{
				if (mouse.get_mdirection() == WEST)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
						break;
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					std::cerr << "Moving Forward!!!!"
							  << "\n";
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
				else if (mouse.get_mdirection() == NORTH)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
						break;
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					std::cerr << "Turning Left!!!!"
							  << "\n";
					mouse.turn_left();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
				else if (mouse.get_mdirection() == EAST)
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'n');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(NORTH, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
					}
					std::cerr << "Moving Backwards!!!!"
							  << "\n";
					mouse.turn_right();
					mouse.turn_right();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
				else
				{
					if (API::wallFront())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 's');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(SOUTH, true);
						std::cerr << "Wall detected in Front!!!!"
								  << "\n";
					}
					if (API::wallLeft())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'e');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(EAST, true);
						std::cerr << "Wall detected in Left!!!!"
								  << "\n";
					}
					if (API::wallRight())
					{
						API::setWall(mouse.get_mx(), mouse.get_my(), 'w');
						mouse.get_maze(mouse.get_mx(), mouse.get_my())->set_wall(WEST, true);
						std::cerr << "Wall detected in Right!!!!"
								  << "\n";
						break;
					}
					std::cerr << "Turning Right !!!!"
							  << "\n";
					mouse.turn_right();
					mouse.move_forward();
					if (!mouse.is_in_vector(stk_inv.top(), v))
					{
						v.push_back(stk_inv.top());
						v1.push_back(stk_inv.top());
					}
				}
			}

			if (!stk_len3 == 0)
				stk_len3--;
			if (stk_len3 == 0)
			{
				s.push(stk_inv.top());
			}
			stk_inv.pop();
		}
	}
	// Draw the final path in blue 
	while (!s.empty())
	{
		using namespace std::this_thread;
		using namespace std::chrono;	  
		sleep_for(nanoseconds(100000000));
		API::setColor(s.top().get_x(), s.top().get_y(), 'b');
		stk_inv.push(s.top());
		s.pop();
	}

	// for(;;){

	// 	std::cerr<<"──────────────────────────────────────────────────────────────────────────────"<<std::flush;
	// 	std::cerr<<"─██████████████─██████████████─██████████████─██████────────────██████─██████─"<<std::flush;
	// 	std::cerr<<"─██░░░░░░░░░░██─██░░░░░░░░░░██─██░░░░░░░░░░██─██░░██────────────██░░██─██░░██─"<<std::flush;
	// 	std::cerr<<"─██░░██████████─██░░██████░░██─██░░██████░░██─██░░██────────────██░░██─██░░██─"<<std::flush;
	// 	std::cerr<<"─██░░██─────────██░░██──██░░██─██░░██──██░░██─██░░██────────────██░░██─██░░██─"<<std::flush;
	// 	std::cerr<<"─██░░██─────────██░░██──██░░██─██░░██████░░██─██░░██────────────██░░██─██░░██─"<<std::flush;
	// 	std::cerr<<"─██░░██──██████─██░░██──██░░██─██░░░░░░░░░░██─██░░██────────────██░░██─██░░██─"<<std::flush;
	// 	std::cerr<<"─██░░██──██░░██─██░░██──██░░██─██░░██████░░██─██░░██────────────██████─██████─"<<std::flush;
	// 	std::cerr<<"─██░░██──██░░██─██░░██──██░░██─██░░██──██░░██─██░░██──────────────────────────"<<std::flush;
	// 	std::cerr<<"─██░░██████░░██─██░░██████░░██─██░░██──██░░██─██░░██████████────██████─██████─"<<std::flush;
	// 	std::cerr<<"─██░░░░░░░░░░██─██░░░░░░░░░░██─██░░██──██░░██─██░░░░░░░░░░██────██░░██─██░░██─"<<std::flush;
	// 	std::cerr<<"─██████████████─██████████████─██████──██████─██████████████────██████─██████─"<<std::flush;
	// 	std::cerr<<"──────────────────────────────────────────────────────────────────────────────"<<std::flush;
	// }

	std::cerr << "██████╗░███████╗░█████╗░░█████╗░██╗░░██╗███████╗██████╗░  ░██████╗░░█████╗░░█████╗░██╗░░░░░  ██╗██╗"
			  << "\n";
	std::cerr << "██╔══██╗██╔════╝██╔══██╗██╔══██╗██║░░██║██╔════╝██╔══██╗  ██╔════╝░██╔══██╗██╔══██╗██║░░░░░  ██║██║"
			  << "\n";
	std::cerr << "██████╔╝█████╗░░███████║██║░░╚═╝███████║█████╗░░██║░░██║  ██║░░██╗░██║░░██║███████║██║░░░░░  ██║██║"
			  << "\n";
	std::cerr << "██╔══██╗██╔══╝░░██╔══██║██║░░██╗██╔══██║██╔══╝░░██║░░██║  ██║░░╚██╗██║░░██║██╔══██║██║░░░░░  ╚═╝╚═╝"
			  << "\n";
	std::cerr << "██║░░██║███████╗██║░░██║╚█████╔╝██║░░██║███████╗██████╔╝  ╚██████╔╝╚█████╔╝██║░░██║███████╗  ██╗██╗"
			  << "\n";
	std::cerr << "╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝░╚════╝░╚═╝░░╚═╝╚══════╝╚═════╝░  ░╚═════╝░░╚════╝░╚═╝░░╚═╝╚══════╝  ╚═╝╚═╝"
			  << "\n";
}