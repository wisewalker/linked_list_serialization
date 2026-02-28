#include <iostream>
#include <string>

#include "linkedList.h"

void test_1()
{
    std::cout << "\n----------------------------------------\n[!!!]Test-1 has started!\n";

    //create list and supply data source/target files for it
    LinkedList list_1("./data/inlet_1.in", "./data/outlet_1.out");

    //init filling the list
    list_1.fillList();

    //test printing forward/backward (by using prepared functions)
    list_1.printListForward();
    list_1.printListBackward();

    std::cout << "[!]Print edited list manually!" << std::endl;
    //..or accessing list directly to both read/edit its contents
    auto head = list_1[0];
    auto curr = head;
    int counter = 0;
    while (curr)
    {
        if (counter++ == 2)
            curr->data = "cool carrot";

        std::cout << "\tNode(" << curr << "-->" << curr->data << ", rand_linked: "
            << curr->rand << "-->" << (curr->rand ? curr->rand->data : "empty") << ")\n";
        curr = curr->next;
    }

    //finally, clear the created list
    list_1.clearList();

    std::cout << "[!!!]Test-1 has finished!" << std::endl;
}

void test_2()
{
    std::cout << "\n----------------------------------------\n[!!!]Test-2 has started!\n";

    //create another list and BUT not supply data source/target files for it
    LinkedList list_2("", "");

    //do it manually
    list_2.setInletFilePath("./data/inlet_2.in");
    list_2.setOutletFilePath("./data/outlet_2.out");

    //init filling the list
    list_2.fillList();

    //test printing forward
    list_2.printListForward();

    //then clear it
    list_2.clearList();

    //and fill it with use of another source file (with accompaying personal output file)
    list_2.setInletFilePath("./data/inlet_3.in");
    list_2.setOutletFilePath("./data/outlet_3.out");
    list_2.fillList();

    //serialize formed list into provided output file (outlet_1.out)
    list_2.serializeList();

    //and deserialize it (also clear list contents, as we do not need it anymore)
    list_2.deserializeList(std::string());

    //and print list contents again after successful deserialization
    list_2.printListBackward();

    std::cout << "[!!!]Test-2 has finished!" << std::endl;
}

void test_3()
{
    std::cout << "\n----------------------------------------\n[!!!]Test-3 has started!\n";

    //create another list and BUT not supply data source/target files for it
    LinkedList list_3("", "");

    //..do it manually
    list_3.setInletFilePath("./data/inlet_4.in");
    list_3.setOutletFilePath("./data/outlet_4.out");

    //init filling the list
    list_3.fillList();

    //test printing forward
    list_3.printListForward();

    //..do it manually with *HEAD
    std::cout << "[!]Manual list iteration:\n";
    auto head = list_3[0];
    auto curr = head;
    while (curr)
    {
        std::cout << "\tNode(" << curr << "-->" << curr->data << ", rand_linked: "
            << curr->rand << "-->" << (curr->rand ? curr->rand->data : "empty") << ")\n";
        curr = curr->next;
    }

    //serialize formed list into provided output file (outlet_4.out)
    list_3.serializeList();

    //and deserialize it
    list_3.deserializeList(std::string());

    //and print list contents again after successful deserialization
    list_3.printListForward();

    std::cout << "[!!!]Test-3 has finished!" << std::endl;
}

int main()
{
    std::cout << "[!!]Program has started!\n";

    //Perform tests
    test_1();
    test_2();
    test_3();

    std::cout << "[!!]Program has finished!" << std::endl;
    return 0;
}