/**
 * @file BoardFactory.h
 * @author John Korreck
 *
 *
 */
 
#ifndef BOARDFACTORY_H
#define BOARDFACTORY_H

class Board;

class BoardFactory {
private:

public:
 std::shared_ptr<Board> Create(std::wstring resourcesDir);
};



#endif //BOARDFACTORY_H
