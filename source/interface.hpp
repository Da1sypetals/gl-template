#include <vector>
#include <glm/vec3.hpp>
#include "memory"

#include "SolverPreprocesser.hpp"

class Interface {

    std::shared_ptr<H_Solver> hSolver;

public:
    void init(std::vector<glm::vec3> &vertices, std::vector<glm::ivec3> &indices) {

        // ---

        int nside;
        float size;
        float k;
        int n_iter;

        std::cout << "n of vertex each side, cloth size, stiffness k, number of iterations / frame" << std::endl;
        std::cin >> nside >> size >> k >> n_iter;


        // ---

        std::shared_ptr<Cloth> cloth = std::make_shared<Cloth>(nside, size, k);

        hSolver = std::make_shared<H_Solver>(cloth, n_iter);

        std::shared_ptr<SolverPreprocessor> pre = std::make_shared<SolverPreprocessor>(hSolver);
        pre->Init();
        hSolver->AddFixed(0, 0);
        hSolver->AddFixed(0, nside - 1);


        // init vertices and indices
        vertices.resize(cloth->numVertex);
        indices.resize(0);
        indices.reserve((cloth->nside - 1) * (cloth->nside - 1) * 3);

        for (int irow = 0; irow < cloth->nside - 1; irow++) {
            for (int icol = 0; icol < cloth->nside - 1; icol++) {

                int i1 = cloth->index(irow, icol);
                int i2 = cloth->index(irow, icol + 1);
                int i3 = cloth->index(irow + 1, icol);
                int i4 = cloth->index(irow + 1, icol + 1);

                indices.push_back({i1, i2, i3});
                indices.push_back({i1, i3, i4});
                indices.push_back({i1, i2, i4});

            }
        }


    }

    void update(std::vector<glm::vec3> &vertices) {

        hSolver->Step();

        // copy data
        for (int i = 0; i < hSolver->cloth->numVertex; i++) {
            vertices[i] = {hSolver->x[3 * i], hSolver->x[3 * i + 1], hSolver->x[3 * i + 2]};
        }

    }
};



