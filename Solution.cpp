#include<iostream>
#include<vector>
#include<set>



using field = std::vector<std::vector<int>>;

class Solver{
public:
    field start_field;
    std::vector<std::pair<int, int>> pos;
    int heuristic_const;     
    Solver(){
        pos.resize(16);
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                pos[i * 4 + j + 1] = {i, j};
            }
        }
    }
    void read(){
        start_field.resize(4);
        for (auto& cur_str : start_field){
            cur_str.resize(4);
        }
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                std::cin >> start_field[i][j];
            }
        }
    }
    std::pair<int, int> find_pos_zero(const field& a){
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                if (a[i][j] == 0){
                    return {i, j};
                }
            }
        }
        return {0, 0};
    }
    bool solution_existence(){
        std::vector<int> cells(16);
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                cells[i * 4 + j] = start_field[i][j];
            }
        }
        
        int number_invers = 0;
        for (int i = 0; i < 16; i++){
	        if (cells[i] > 0){
		        for (int j = 0; j < i; j++){
			        if (cells[i] < cells[j]){
				        number_invers++;
                    }
                }
            }
            else{
                number_invers += (1 + i / 4);
            }
        }

        if (number_invers % 2 == 1){
            return false;
        }
        else{
            return true;
        }   
    }

    int heuristic_function(const field& a){
        int sum = 0;
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                if (a[i][j] != 0){
                    sum += abs(i - pos[a[i][j]].first) + abs(j - pos[a[i][j]].second);
                }
            }
        }
        return heuristic_const * sum;
    }

    std::pair<int, int> move_cell(const std::pair<int,int>& cell, const std::pair<int,int>& bias){
        return {cell.first + bias.first, cell.second + bias.second};
    }

    bool check_correct(const std::pair<int, int>& cell){
        return cell.first >= 0 && cell.first < 4 
                && cell.second >= 0 && cell.second < 4;
    }

    std::vector<field> move_cells(const field& a){
        std::vector<std::pair<int,int>> biases = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        
        std::pair<int,int> pos_zero = find_pos_zero(a);
        std::vector<field> ans;
            
        for (const auto& cur_bias : biases){
            std::pair<int,int> new_pos_zero = move_cell(pos_zero, cur_bias);
            if (check_correct(new_pos_zero)){
                field new_ans = a;
                std::swap(new_ans[pos_zero.first][pos_zero.second], 
                        new_ans[new_pos_zero.first][new_pos_zero.second]);
                ans.push_back(new_ans);
            }
        }

        return ans; 
    }

    void algorithm_a(){
        std::set<std::pair<int, field>> s;
        std::set<field> used;
        used.insert(start_field);
        s.insert({heuristic_function(start_field), start_field});
        int cur_dist = 0;
        while (!s.empty()){
            auto vertex = *s.begin();
            s.erase(s.begin());
            field cur_field = vertex.second;
            cur_dist = vertex.first - heuristic_function(vertex.second);
            if (cur_dist == vertex.first){
                break;
            }
            std::vector<field> new_fields = move_cells(cur_field);
            for (const auto& new_field : new_fields){
                if (used.find(new_field) == used.end()){
                    used.insert(new_field);
                    s.insert({cur_dist + 1 + heuristic_function(new_field), new_field});
                }
            }
        }
        std::cout << cur_dist << std::endl;
    }
};

signed main(){
    Solver solver;
    solver.read();
    if (!solver.solution_existence()){
        std::cout << "No_solution" << std::endl;
        return 0;
    }
    
    std::cin >> solver.heuristic_const;
    
    solver.algorithm_a(); 
    
    return 0;
}
