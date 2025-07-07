/*

2 ANSWER WHILE STATEMENT

----

    answer = 0;
    while (true) {

        std::cin >> answer;

        if (std::cin.fail() || (parameters)) {
            std::cout << "error message here\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {

            if (answer == 1) {

                //do if answer = 1

            } else if (answer == 2) {

                //do if answer = 2

            }
            break;
        }

    }
*/