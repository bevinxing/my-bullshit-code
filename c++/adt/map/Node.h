//
// Created by liang on 17/11/23.
//

#ifndef MAP_NODE_H
#define MAP_NODE_H

/*
 *          A
 *        /   \
 *      B      D
 *    /  \    /  \
 *   C   F   G - H
 *    \ /
 *     E
 *    A  B  C  D  E  F  G  H
 * A  0  1  0  1  0  0  0  0
 * B  1  0  1  0  0  1  0  0
 * C  0  1  0  0  1  0  0  0
 * D  1  0  0  0  0  0  1  1
 * E  0  0  1  0  0  1  0  0
 * F  0  1  0  0  1  0  0  0
 * G  0  0  0  1  0  0  0  1
 * H  0  0  0  1  0  0  1  0
 */
class Node
{
public:
    Node(char data=0);
    char m_cData;
    bool m_bIsVisited;
};

#endif //MAP_NODE_H
