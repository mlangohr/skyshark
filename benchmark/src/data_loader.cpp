#include <iostream>
#include <fstream>

#include "config.h"
#include "state.cpp"

namespace Benchmark {
    class DataLoader {
    private:
        std::fstream infile;
    public:
        DataLoader(Benchmark::Config config){
            infile =  std::fstream(config.get_input_file(), std::ios::binary | std::ios::in);
        }

        ~DataLoader(){
            if(infile && infile.is_open()){
                infile.close();
                delete &infile;
            }
        }

        std::optional<Benchmark::State> get() {
            Benchmark::State state;
            if (infile.read(reinterpret_cast<char*>(&state), sizeof(state))) {
                return state;
            }
            return {}; // Returns an empty optional if read is unsuccessful
        }

        void close_file(){
            this->infile.close();
        }
    };
}
