/**************************************************************************
 *   This file is part of TURING.                                         *
 *                                                                        *
 *   Author: Ivo Filot <ivo@ivofilot.nl>                                  *
 *                                                                        *
 *   TURING is free software:                                             *
 *   you can redistribute it and/or modify it under the terms of the      *
 *   GNU General Public License as published by the Free Software         *
 *   Foundation, either version 3 of the License, or (at your option)     *
 *   any later version.                                                   *
 *                                                                        *
 *   TURING is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#include "reaction_system.h"

/**
 * @brief      Constructs the object.
 */
ReactionSystem::ReactionSystem() {

}

/**
 * @brief      random initialization
 *
 * @param      a     Concentration matrix A
 * @param      b     Concentration matrix B
 */
void ReactionSystem::init_random(MatrixXXd& a, MatrixXXd& b, double ca, double cb, double delta) const {
    unsigned int width = a.cols();
    unsigned int height = a.rows();

    a = MatrixXXd::Zero(height, width);
    b = MatrixXXd::Zero(height, width);

    for(unsigned int i=0; i<height; i++) {
        for(unsigned int j=0; j<width; j++) {
            a(i,j) = ca + this->uniform_dist() * delta;
            b(i,j) = cb + this->uniform_dist() * delta;
        }
    }
}

/**
 * @brief      Make a single sphere in the center of the system
 *
 * @param      a     Concentration matrix A
 * @param      b     Concentration matrix B
 * @param[in]  ca    concentration of A in center
 * @param[in]  cb    concentration of B in center
 */
void ReactionSystem::init_central_circle(MatrixXXd& a, MatrixXXd& b, double ca, double cb) const {
    unsigned int width = a.cols();
    unsigned int height = a.rows();

    a = MatrixXXd::Zero(height, width);
    b = MatrixXXd::Zero(height, width);

    for(unsigned int i=0; i<height; i++) {
        for(unsigned int j=0; j<width; j++) {
            double r = (double)width / 16.0;
            double r2 = r * r;
            double dx = (double)(width) / 2.0 - (double)j;
            double dy = (double)(height) / 2.0 - (double)i;
            if( (dx * dx + dy * dy) < r2) {
                a(i,j) = ca;
                b(i,j) = cb;
            }
        }
    }
}

/**
 * @brief      Make a two spheres in the center of the system
 *
 * @param      a     Concentration matrix A
 * @param      b     Concentration matrix B
 * @param[in]  ca    concentration of A in center
 * @param[in]  cb    concentration of B in center
 */
void ReactionSystem::init_dual_central_circle(MatrixXXd& a, MatrixXXd& b, double ca, double cb) const {
    unsigned int width = a.cols();
    unsigned int height = a.rows();

    a = MatrixXXd::Zero(height, width);
    b = MatrixXXd::Zero(height, width);

    // make upper circle
    for(unsigned int i=0; i<height; i++) {
        for(unsigned int j=0; j<width; j++) {
            double r = (double)width / 32.0;
            double r2 = r * r;
            double dx = (double)(width) / 2.0 - (double)(j - height / 4);
            double dy = (double)(height) / 2.0 - (double)i;
            if( (dx * dx + dy * dy) < r2) {
                a(i,j) = ca;
                b(i,j) = cb;
            }
        }
    }

    // make lower circle
    for(unsigned int i=0; i<height; i++) {
        for(unsigned int j=0; j<width; j++) {
            double r = (double)width / 32.0;
            double r2 = r * r;
            double dx = (double)(width) / 2.0 - (double)(j + height / 4);
            double dy = (double)(height) / 2.0 - (double)i;
            if( (dx * dx + dy * dy) < r2) {
                a(i,j) = ca;
                b(i,j) = cb;
            }
        }
    }
}

/**
 * @brief      Set random rectangles as initial value
 *
 * @param      a     Concentration matrix A
 * @param      b     Concentration matrix B
 */
void ReactionSystem::init_random_rectangles(MatrixXXd& a, MatrixXXd& b) const {
    unsigned int width = a.cols();
    unsigned int height = a.rows();

    a = MatrixXXd::Ones(height, width) * 0.4201;
    b = MatrixXXd::Ones(height, width) * 0.2878;

    for(unsigned int k=0; k<100; k++) {
        int f = height / 2 + (int)((this->uniform_dist()-0.5) * height * 0.90);
        int g = width / 2 + (int)((this->uniform_dist()-0.5) * width * 0.90);
        double val1 = this->uniform_dist();
        double val2 = this->uniform_dist();
        const int imax = (unsigned int)((this->uniform_dist() * 0.1 * height));
        const int jmax = (unsigned int)((this->uniform_dist() * 0.1 * height));
        for(int i=-imax/2; i<imax/2; i++) {
            for(int j=-jmax/2; j<jmax/2; j++) {
                a(f+i, g+j) = val1;
                b(f+i, g+j) = val2;
            }
        }
    }
}

// Тестовые фигуры. ------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>
struct Circle { double x, y, r; };
void ReactionSystem::init_tst_figure(MatrixXXd& a, MatrixXXd& b) const {
    unsigned int w = a.cols(), h = a.rows(), R = std::min(w, h)/2;
    double ca = 1.0, cb = 0.1;

    a = MatrixXXd::Zero(h, w);
    b = MatrixXXd::Zero(h, w);


    const int MAX_SIZE = 5;
    Circle crc[MAX_SIZE];
    int count = (uniform_dist()*(double)(MAX_SIZE)) + 1;
    if(count > 5) { count = 5; }

    std::cout << "count:" << count;

    for(int i=0; i<count; i++)
        { crc[i] =
            { uniform_dist()*(double)w
            , uniform_dist()*(double)h
            , uniform_dist()*(double)10 }; }

    for(int i=0; i<count; i++) {
        std::cout << "| x:" << crc[i].x
                  << ", y:" << crc[i].y
                  << ", r:" << crc[i].r;
    }// i



    auto IN = [](int x1, int y1, int x2, int y2, int r) -> bool
        { return std::sqrt( std::pow(x1-x2, 2) + std::pow(y1-y2, 2)) <= r; };

    for(unsigned int i=0; i<w; i++) {
        for(unsigned int j=0; j<h; j++) {

            for(int c=0; c<count; c++) {
                if(IN(i, j, crc[c].x, crc[c].y, crc[c].r))
                    { a(i,j) = ca; b(i,j) = cb; }
            }

        }// j
    }// i

}// init_two_rectangles

/**
 * @brief      Make a half screen filling
 *
 * @param      a     Concentration matrix A
 * @param      b     Concentration matrix B
 * @param[in]  ca    concentration of A in center
 * @param[in]  cb    concentration of B in center
 */
void ReactionSystem::init_half_screen(MatrixXXd& a, MatrixXXd& b, double ca, double cb) const {
    unsigned int width = a.cols();
    unsigned int height = a.rows();

    a = MatrixXXd::Zero(height, width);
    b = MatrixXXd::Zero(height, width);

    for(unsigned int i=height/2; i<height; i++) {
        for(unsigned int j=0; j<width; j++) {
            a(i,j) = ca;
        }
    }

    for(unsigned int i=0; i<height; i++) {
        for(unsigned int j=0; j<width/2; j++) {
            b(i,j) = cb;
        }
    }
}

/**
 * @brief      Parse parameters
 *
 * @param[in]  params  string containing list of parameters
 *
 * @return     unordered map with the parameters
 */
std::unordered_map<std::string, double> ReactionSystem::parse_parameters(const std::string& params) const {
    std::vector<std::string> pieces;
    boost::split(pieces, params, boost::is_any_of(";"), boost::token_compress_on);

    std::unordered_map<std::string, double> map;

    for(const std::string& piece : pieces) {
        std::vector<std::string> vars;
        boost::split(vars, piece, boost::is_any_of("="), boost::token_compress_on);
        if(vars.size() != 2) {
            std::cerr << "Piece: " << piece << std::endl;
            std::cerr << "Var size: " << vars.size() << std::endl;
            throw std::runtime_error("Invalid params list encountered: " + params);
        }

        map.emplace(vars[0], boost::lexical_cast<double>(vars[1]));
    }

    return map;
}
