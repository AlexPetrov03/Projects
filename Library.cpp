#include "Library.h"
Literature::Literature()
{
    title = new char[1];
    title[0] = '\0';
    description = new char[1];
    description[0] = '\0';
    id = 0;
    year = 0;
}
Literature::Literature(const char *title, const char *description, unsigned int id, unsigned int year) : title(nullptr), description(nullptr)
{
    setTitle(title);
    setDescription(description);
    setId(id);
    setYear(year);
}
Literature::Literature(const Literature &other)
{
    copyFrom(other);
}
void Literature::copyFrom(const Literature &other)
{
    this->title = new char[strlen(other.getTitle()) + 1];
    strcpy(this->title, other.getTitle());
    this->description = new char[strlen(other.getDescription()) + 1];
    strcpy(this->description, other.getDescription());
    this->id = other.getId();
    this->year = other.getYear();
}
Literature &Literature::operator=(const Literature &other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}
void Literature::free()
{
    delete[] title;
    delete[] description;
}
Literature::~Literature()
{
    free();
}
void Literature::setTitle(const char *title)
{
    if (this->title == title)
        return;
    delete[] this->title;
    this->title = new char[strlen(title) + 1];
    strcpy(this->title, title);
}
void Literature::setDescription(const char *description)
{
    if (this->description == description)
        return;
    delete[] this->description;
    this->description = new char[strlen(description) + 1];
    strcpy(this->description, description);
}
void Literature::setId(unsigned int id)
{
    this->id = id;
}
void Literature::setYear(unsigned int year)
{
    this->year = year;
}
const char *Literature::getTitle() const
{
    return title;
}
const char *Literature::getDescription() const
{
    return description;
}
unsigned int Literature::getId() const
{
    return id;
}
unsigned int Literature::getYear() const
{
    return year;
}
const char *Literature::getType() const
{
    return "Literature";
}
Book::Book(const char *author, const char *title, const char *publishingHouse, unsigned int genre, const char *description, unsigned int id, unsigned int year) : Literature(title, description, id, year), author(nullptr), publishingHouse(nullptr)
{
    setAuthor(author);
    setPublishingHouse(publishingHouse);
    setGenre(genre);
}
Book::Book() : Literature()
{
    author = new char[1];
    author[0] = '\0';
    publishingHouse = new char[1];
    publishingHouse[0] = '\0';
    genre = 0;
}
Book::Book(const Book &other) : Literature(other)
{
    copyFrom(other);
}
Book &Book::operator=(const Book &other)
{
    if (this != &other)
    {
        Literature::operator=(other);
        free();
        copyFrom(other);
    }
    return *this;
}
void Book::free()
{
    delete[] author;
    delete[] publishingHouse;
}
Book::~Book()
{
    free();
}
void Book::setAuthor(const char *author)
{
    if (this->author == author)
        return;
    delete[] this->author;
    this->author = new char[strlen(author) + 1];
    strcpy(this->author, author);
}
void Book::setGenre(unsigned int genre)
{
    this->genre = genre;
}
void Book::setPublishingHouse(const char *publishingHouse)
{
    if (this->publishingHouse == publishingHouse)
        return;
    delete[] this->publishingHouse;
    this->publishingHouse = new char[strlen(publishingHouse) + 1];
    strcpy(this->publishingHouse, publishingHouse);
}
void Book::setCharacteristics(unsigned int characteristics)
{
    genre = (genre & 0xFF80) | (characteristics & 0x007F);
}
void Book::setType(unsigned int type)
{
    genre = (genre & 0xFF0F) | ((type & 0x000F) << 4);
}
void Book::setTargetAudience(unsigned int targetAudience)
{
    genre = (genre & 0xFCFF) | ((targetAudience & 0x000F) << 8);
}
void Book::setEvenOdd()
{
    int bitCount = 0;
    unsigned int tempGenre = genre;
    while (tempGenre)
    {
        bitCount += tempGenre & 1;
        tempGenre >>= 1;
    }
    genre = (genre & 0x7FFF) | ((bitCount % 2) << 15);
}
const char *Book::getAuthor() const
{
    return author;
}
const char *Book::getPublishingHouse() const
{
    return publishingHouse;
}
unsigned int Book::getGenre() const
{
    return genre;
}
void Book::copyFrom(const Book &other)
{
    setAuthor(other.getAuthor());
    setPublishingHouse(other.getPublishingHouse());
    setGenre(other.getGenre());
}

PeriodicalLiterature::PeriodicalLiterature(const char *title, Periodicity periodicity, const char *description, unsigned int issue, unsigned int id, unsigned int year) : Literature(title, description, id, year)
{
    this->periodicity = periodicity;
    this->issue = issue;
}
PeriodicalLiterature::PeriodicalLiterature(const PeriodicalLiterature &other) : Literature(other)
{
    copyFrom(other);
}
PeriodicalLiterature::PeriodicalLiterature() : Literature()
{
    periodicity = Periodicity::NOT_SET;
    issue = 0;
}
PeriodicalLiterature &PeriodicalLiterature::operator=(const PeriodicalLiterature &other)
{
    if (this != &other)
    {
        Literature::operator=(other);
        copyFrom(other);
    }
    return *this;
}
void PeriodicalLiterature::copyFrom(const PeriodicalLiterature &other)
{
    setPeriodicity(other.getPeriodicity());
    setIssue(other.getIssue());
}
void PeriodicalLiterature::setPeriodicity(Periodicity periodicity)
{
    this->periodicity = periodicity;
}
void PeriodicalLiterature::setIssue(unsigned int issue)
{
    this->issue = issue;
}
Periodicity PeriodicalLiterature::getPeriodicity() const
{
    return periodicity;
}
unsigned int PeriodicalLiterature::getIssue() const
{
    return issue;
}
const char *Book::getType() const
{
    return "Book";
}
unsigned int Book::getCharacteristics() const
{
    return genre & 0x007F;
}
unsigned int Book::getGenreType() const
{
    return (genre & 0x00F0) >> 4;
}
unsigned int Book::getTargetAudience() const
{
    return (genre & 0x0F00) >> 8;
}
unsigned int Book::getEvenOdd() const
{
    return (genre & 0x8000) >> 15;
}
const char *PeriodicalLiterature::getType() const
{
    return "Periodical Literature";
}
Comic::Comic(const char *author, const char *title, const char *publishingHouse, unsigned int genre, const char *description, Periodicity periodicity, unsigned int issue, unsigned int id, unsigned int year) : Literature(title, description, id, year)
{
    setAuthor(author);
}
Comic::Comic() : Literature()
{
    author = new char[1];
    author[0] = '\0';
    publishingHouse = new char[1];
    publishingHouse[0] = '\0';
    genre = 0;
    periodicity = Periodicity::NOT_SET;
    issue = 0;
}
Comic::~Comic()
{
    delete[] author;
    delete[] publishingHouse;
}
void Comic::setAuthor(const char *author)
{
    if (this->author == author)
        return;
    delete[] this->author;
    this->author = new char[strlen(author) + 1];
    strcpy(this->author, author);
}
void Comic::setGenre(unsigned int genre)
{
    this->genre = genre;
}
void Comic::setPublishingHouse(const char *publishingHouse)
{
    if (this->publishingHouse == publishingHouse)
        return;
    delete[] this->publishingHouse;
    this->publishingHouse = new char[strlen(publishingHouse) + 1];
    strcpy(this->publishingHouse, publishingHouse);
}
void Comic::setCharacteristics(unsigned int characteristics)
{
    genre = (genre & 0xFF80) | (characteristics & 0x007F);
}
void Comic::setType(unsigned int type)
{
    genre = (genre & 0xFF0F) | ((type & 0x000F) << 4);
}
void Comic::setTargetAudience(unsigned int targetAudience)
{
    genre = (genre & 0xFCFF) | ((targetAudience & 0x000F) << 8);
}
void Comic::setEvenOdd()
{
    int bitCount = 0;
    unsigned int tempGenre = genre;
    while (tempGenre)
    {
        bitCount += tempGenre & 1;
        tempGenre >>= 1;
    }
    genre = (genre & 0x7FFF) | ((bitCount % 2) << 15);
}
const char *Comic::getAuthor() const
{
    return author;
}
const char *Comic::getPublishingHouse() const
{
    return publishingHouse;
}
unsigned int Comic::getGenre() const
{
    return genre;
}
unsigned int Comic::getCharacteristics() const
{
    return genre & 0x007F;
}
unsigned int Comic::getGenreType() const
{
    return (genre & 0x00F0) >> 4;
}
unsigned int Comic::getTargetAudience() const
{
    return (genre & 0x0F00) >> 8;
}
unsigned int Comic::getEvenOdd() const
{
    return (genre & 0x8000) >> 15;
}
const char *Comic::getType() const
{
    return "Comic";
}

User::User(const char *name) : name(nullptr)
{
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
    capacity = 10;
    takenLiterature = new unsigned int[capacity];
    takenLiteratureSize = 0;
    returnedLiterature = new unsigned int[capacity];
    returnedLiteratureSize = 0;
}
User::User()
{
    name = new char[1];
    name[0] = '\0';
    capacity = 10;
    takenLiterature = new unsigned int[capacity];
    takenLiteratureSize = 0;
    returnedLiterature = new unsigned int[capacity];
    returnedLiteratureSize = 0;
    numberOfTakenLiterature = 0;
}
User::User(const User &other)
{

    copyFrom(other);
}
User::~User()
{
    free();
}
void User::free()
{
    delete[] name;
    delete[] takenLiterature;
    delete[] returnedLiterature;
}
void User::setName(const char *name)
{
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}
void User::setTakenLiteratureSize(unsigned int takenLiteratureSize)
{
    this->takenLiteratureSize = takenLiteratureSize;
}
void User::setReturnedLiteratureSize(unsigned int returnedLiteratureSize)
{
    this->returnedLiteratureSize = returnedLiteratureSize;
}
void User::setCapacity(unsigned int capacity)
{
    this->capacity = capacity;
}
char *User::getName() const
{
    return name;
}
unsigned int User::getTakenLiterature(unsigned int id) const
{
    return takenLiterature[id];
}
unsigned int User::getTakenLiteratureSize() const
{
    return takenLiteratureSize;
}
unsigned int User::getReturnedLiterature(unsigned int id) const
{
    return returnedLiterature[id];
}
unsigned int User::getReturnedLiteratureSize() const
{
    return returnedLiteratureSize;
}
unsigned int User::getNumberOfTakenLiterature() const
{
    return numberOfTakenLiterature;
}
unsigned int User::getCapacity() const
{
    return capacity;
}
void User::takeLiterature(unsigned int id)
{
    if(numberOfTakenLiterature == 5){
        std::cout << "Error. Maximum number of literature taken reached." << std::endl;
        return;
    }
    takenLiterature[takenLiteratureSize] = id;
        takenLiteratureSize++;
        numberOfTakenLiterature++;
}
void User::returnLiterature(unsigned int id)
{
    int pos = -1;
    for (std::size_t i = 0; i < takenLiteratureSize; i++)
    {
        if (id = takenLiterature[i])
        {
            pos = i;
            break;
        }
    }
    if (pos != -1)
    {
        for (std::size_t i = pos; i < takenLiteratureSize - 1; i++)
        {
            takenLiterature[i] = takenLiterature[i + 1];
        }
        takenLiterature[takenLiteratureSize - 1] = 0;
        takenLiteratureSize--;
        returnedLiterature[returnedLiteratureSize] = id;
        returnedLiteratureSize++;
        numberOfTakenLiterature--;
    }
    else
    {
        std::cout << "The Literature is not in the possession of this User" << std::endl;
    }
}
void User::copyFrom(const User &other)
{
    setName(other.getName());
    capacity = other.getCapacity();
    takenLiteratureSize = other.getTakenLiteratureSize();
    takenLiterature = new unsigned int[capacity];
    for (std::size_t i = 0; i < takenLiteratureSize; i++)
    {
        takenLiterature[i] = other.getTakenLiterature(i);
    }
    returnedLiterature = new unsigned int[capacity];
    returnedLiteratureSize = other.getReturnedLiteratureSize();
    for (std::size_t i = 0; i < returnedLiteratureSize; i++)
    {
        returnedLiterature[i] = other.getReturnedLiterature(i);
    }
}
User &User::operator=(const User &other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return (*this);
}
bool User::operator==(const User &other)
{
    return !strcmp(name, other.getName());
}
bool User::operator!=(const User &other)
{
    return strcmp(name, other.getName());
}
bool User::operator<(const User &other)
{
    return returnedLiteratureSize < other.getReturnedLiteratureSize();
}
bool User::operator<=(const User &other)
{
    return returnedLiteratureSize <= other.getReturnedLiteratureSize();
}
bool User::operator>(const User &other)
{
    return returnedLiteratureSize > other.getReturnedLiteratureSize();
}
bool User::operator>=(const User &other)
{
    return returnedLiteratureSize >= other.getReturnedLiteratureSize();
}
User &User::operator+(const Literature &literature)
{
    takeLiterature(literature.getId());
    return (*this);
}
User &User::operator+=(const Literature &literature)
{
    takeLiterature(literature.getId());
    return (*this);
}
User &User::operator-(const Literature &literature)
{
    returnLiterature(literature.getId());
    return (*this);
}
void User::saveUserToBinaryFile(const char *fileName)
{
    std::ofstream file(fileName, std::ios::binary);

    if (!file)
    {
        std::cout << "Failed to open the file for writing: " << fileName << std::endl;
        return;
    }
    file.write(reinterpret_cast<const char *>(this), sizeof(User));
    file.close();
}
User &User::operator-=(const Literature &literature)
{
    returnLiterature(literature.getId());
    return (*this);
}
int User::operator[](unsigned int id)
{
    if (hasTakenLiterature(id))
        return 1;
    else if (hasReturnedLiterature(id))
        return 0;
    else
        return -1;
}
void User::setNumberOfTakenLiterature(unsigned int numberOfTakenLiterature)
{
    this->numberOfTakenLiterature = numberOfTakenLiterature;
}
bool User::hasTakenLiterature(unsigned int id)
{
    for (std::size_t i = 0; i < takenLiteratureSize; i++)
    {
        if (takenLiterature[i] == id)
            return true;
    }
    return false;
}
bool User::hasReturnedLiterature(unsigned int id)
{
    for (std::size_t i = 0; i < returnedLiteratureSize; i++)
    {
        if (returnedLiterature[i] == id)
            return true;
    }
    return false;
}
Library::Library()
{
    this->capacity = 10;
    this->userCapacity = 10;
    this->size = 0;
    this->userSize = 0;
    this->collection = new Literature *[capacity];
    takenLiterature = new unsigned int[capacity];
    this->takenLiteratureSize = 0;
    for (std::size_t i = 0; i < capacity; i++)
    {
        collection[i] = nullptr;
    }
    this->users = new User *[userCapacity];
    for (std::size_t i = 0; i < userCapacity; i++)
    {
        users[i] = nullptr;
    }
}
Library::~Library()
{
    for (std::size_t i = 0; i < size; i++)
    {
        delete[] collection[i];
    }
    for (std::size_t i = 0; i < userSize; i++)
    {
        delete[] users[i];
    }
    delete[] users;
    delete[] collection;
    delete[] takenLiterature;
}
Literature **Library::getCollection() const
{
    return collection;
}
void Library::addLiterature(const Literature &literature)
{
    if (size == capacity)
    {
        resizeCollection();
    }
    if (strcmp(literature.getType(), "Book") == 0)
    {
        collection[size] = new Book(dynamic_cast<const Book &>(literature));
        size++;
    }
    else if (strcmp(literature.getType(), "Periodical Literature") == 0)
    {
        collection[size] = new PeriodicalLiterature(dynamic_cast<const PeriodicalLiterature &>(literature));
        size++;
    }
    else if (strcmp(literature.getType(), "Comic") == 0)
    {
        collection[size] = new Comic(dynamic_cast<const Comic &>(literature));
        size++;
    }
}
void Library::resizeCollection()
{
    capacity *= 2;

    Literature **newCollection = new Literature *[capacity];
    unsigned int *newTakenLiterature = new unsigned int[capacity];

    for (std::size_t i = 0; i < size; i++)
    {
        newCollection[i] = collection[i];
        delete[] collection[i];
    }
    for (std::size_t i = 0; i < takenLiteratureSize; i++)
    {
        newTakenLiterature[i] = takenLiterature[i];
    }

    delete[] collection;
    delete[] takenLiterature;

    collection = newCollection;
    takenLiterature = newTakenLiterature;
}
unsigned int Library::getSize() const
{
    return size;
}
unsigned int Library::getCapacity() const
{
    return capacity;
}
void Library::removeLiterature(unsigned int id)
{
    int position = -1;
    for (std::size_t i = 0; i < size; i++)
    {
        if (collection[i]->getId() == id)
        {
            position = i;
            break;
        }
    }

    if (position != -1)
    {
        delete collection[position];
        collection[position] = nullptr;

        for (std::size_t i = position; i < size - 1; i++)
        {
            collection[i] = collection[i + 1];
        }

        collection[size - 1] = nullptr;
        size--;
    }
    else
    {
        std::cout << "Literature with ID " << id << " not found in the library." << std::endl;
    }
}
void Library::resizeUsers()
{
    userCapacity *= 2;
    User **newUsers = new User *[userCapacity];
    for (std::size_t i = 0; i < userSize; i++)
    {
        newUsers[i] = users[i];
        delete[] users[i];
    }
    delete[] users;
    users = newUsers;
}
void Library::addOrUpdateUser(const User &user)
{
    if (userExists(user.getName()))
    {
        removeUser(user.getName());
    }
    if (userSize == capacity)
        resizeUsers();
    users[userSize] = new User(user);
    userSize++;
}
void Library::removeUser(const char *name)
{
    int userIndex = -1;
    for (unsigned int i = 0; i < userSize; i++)
    {
        if (strcmp(users[i]->getName(), name) == 0)
        {
            userIndex = i;
            break;
        }
    }

    if (userIndex != -1)
    {
        for (unsigned int i = userIndex; i < userSize - 1; i++)
        {
            delete users[i];
            users[i] = new User(*users[i + 1]);
        }
        userSize--;

        delete users[userSize];
        users[userSize] = nullptr;
    }
    else
    {
        std::cout << "User '" << name << "' does not exist in the library." << std::endl;
    }
}
Literature &Library::getLiteratureById(unsigned int id)
{
    for (std::size_t i = 0; i < size; i++)
    {
        if (id == collection[i]->getId())
            return *collection[i];
    }
}
void Library::takeLiterature(User &user, unsigned int id)
{
    user + getLiteratureById(id);
    addOrUpdateUser(user);
    takenLiterature[takenLiteratureSize] = id;
    takenLiteratureSize++;
}
void Library::returnLiterature(User &user, unsigned int id)
{
    if (!user.hasTakenLiterature(id))
    {
        std::cout << "The Literature is not in the possession of this user." << std::endl;
        return;
    }
    user - getLiteratureById(id);
    addOrUpdateUser(user);
    int pos = -1;
    for (std::size_t i = 0; i < takenLiteratureSize; i++)
    {
        if (takenLiterature[i] == id)
            pos = i;
    }
    for (std::size_t i = pos; i < takenLiteratureSize - 1; i++)
    {
        takenLiterature[i] = takenLiterature[i + 1];
    }
    takenLiterature[takenLiteratureSize] = 0;
    takenLiteratureSize--;
}
bool Library::compareLiterature(const Literature &literature1, const Literature &literature2) const
{
    if (literature1.getYear() == literature2.getYear())
    {
        return literature1.getTitle() < literature2.getTitle();
    }
    return literature1.getYear() < literature2.getYear();
}
bool Library::comparePeriodicals(const PeriodicalLiterature &per1, const PeriodicalLiterature &per2) const
{
    if (per1.getYear() == per2.getYear())
    {
        if (per1.getTitle() == per2.getTitle())
        {
            if (per1.getIssue() == per2.getIssue())
            {
                return per1.getTitle() < per2.getTitle();
            }
            return per1.getIssue() > per2.getIssue();
        }
        return per1.getTitle() < per2.getTitle();
    }
    return per1.getYear() < per2.getYear();
}
void Library::printLibraryLiterature() const
{
    const unsigned int maxLiterature = size;
    Literature *literature[maxLiterature];
    int numLiterature = 0;
    for (std::size_t i = 0; i < maxLiterature; i++)
    {
        Literature *item = collection[i];
        if (item != nullptr)
        {
            literature[numLiterature] = item;
            numLiterature++;
        }
    }

    for (std::size_t i = 0; i < numLiterature - 1; i++)
    {
        for (std::size_t j = 0; j < numLiterature - i - 1; j++)
        {
            if (compareLiterature(*literature[j], *literature[j + 1]))
            {
                Literature *temp = literature[j];
                literature[j] = literature[j + 1];
                literature[j + 1] = temp;
            }
        }
    }

    for (std::size_t i = 0; i < numLiterature; i++)
    {
        Literature *item = literature[i];
        std::cout << "Title: " << item->getTitle() << std::endl;
        std::cout << "Type: " << item->getType() << std::endl;
        std::cout << "Description: " << item->getDescription() << std::endl;
        std::cout << "Library Number: " << item->getId() << std::endl;
        std::cout << std::endl;
    }
}
bool Library::userExists(const char *name) const
{
    for (std::size_t i = 0; i < userSize; i++)
    {
        if (strcmp(users[i]->getName(), name) == 0)
        {
            return true;
        }
    }
    return false;
}
void Library::printUsers() const
{
    unsigned int *userCounts = new unsigned int[userSize];
    for (std::size_t i = 0; i < userSize; ++i)
    {
        unsigned int returnedCount = users[i]->getReturnedLiteratureSize();
        userCounts[i] = returnedCount;
    }
    for (std::size_t i = 0; i < userSize - 1; ++i)
    {
        for (std::size_t j = 0; j < userSize - i - 1; ++j)
        {
            if (userCounts[j] < userCounts[j + 1])
            {
                unsigned int tempCount = userCounts[j];
                userCounts[j] = userCounts[j + 1];
                userCounts[j + 1] = tempCount;

                User *tempUser = users[j];
                users[j] = users[j + 1];
                users[j + 1] = tempUser;
            }
        }
    }
    for (std::size_t i = 0; i < userSize; ++i)
    {
        std::cout << "User: " << users[i]->getName() << ", Returned Books: " << userCounts[i] << std::endl;
    }
    delete[] userCounts;
}
void Library::printUsersHaveNotReturned(unsigned int id) const
{
    for (std::size_t i = 0; i < userSize; i++)
    {
        if (users[i]->hasTakenLiterature(id) && !users[i]->hasReturnedLiterature(id))
        {
            std::cout << users[i]->getName() << std::endl;
        }
    }
}
unsigned int Library::getUserIdByName(const char *name) const
{
    for (std::size_t i = 0; i < userSize; i++)
    {
        if (strcmp(users[i]->getName(), name) == 0)
            return i;
    }
}
void Library::readLibraryFromBinaryFile(const char *fileName)
{
    std::ifstream inputFile(fileName, std::ios::binary);
    if (!inputFile)
    {
        std::cout << "Error opening file: " << fileName << std::endl;
        return;
    }
    unsigned int numObjects;
    inputFile.read(reinterpret_cast<char *>(&numObjects), sizeof(numObjects));

    for (std::size_t i = 0; i < numObjects; ++i)
    {
        unsigned int objectType;
        inputFile.read(reinterpret_cast<char *>(&objectType), sizeof(objectType));
        if (objectType == 1)
        {
            Book book;
            inputFile.read(reinterpret_cast<char *>(&book), sizeof(Book));
            addLiterature(book);
        }
        else if (objectType == 2)
        {
            PeriodicalLiterature periodical;
            inputFile.read(reinterpret_cast<char *>(&periodical), sizeof(PeriodicalLiterature));
            addLiterature(periodical);
        }
        else if (objectType == 3)
        {
            Comic comic;
            inputFile.read(reinterpret_cast<char *>(&comic), sizeof(Comic));
            addLiterature(comic);
        }
        else
        {
            std::cout << "Invalid object type: " << objectType << std::endl;
        }
    }
    inputFile.close();
}
void Library::readUsersFromBinaryFile(const char *fileName)
{
    std::ifstream file(fileName, std::ios::binary);

    if (!file)
    {
        std::cout << "Failed to open the file for reading: " << fileName << std::endl;
        return;
    }
    while (file.peek() != EOF)
    {
        User user;
        file.read(reinterpret_cast<char *>(&user), sizeof(User));
        addOrUpdateUser(user);
    }
    file.close();
}
void Literature::saveLiteratureToFile(const char *fileName)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << fileName << std::endl;
        return;
    }
    unsigned int objectType = 0;
    if (dynamic_cast<const Book *>(this) != nullptr)
    {
        objectType = 1;
    }
    else if (dynamic_cast<const PeriodicalLiterature *>(this) != nullptr)
    {
        objectType = 2;
    }
    else if (dynamic_cast<const Comic *>(this) != nullptr)
    {
        objectType = 3;
    }
    file.write(reinterpret_cast<const char *>(&objectType), sizeof(objectType));
    file.write(reinterpret_cast<const char *>(this), sizeof(this));
}
int main()
{
    Library l;
    User u("alex");
    Book b("f", "f", "f", 213, "f", 12, 2003);
    Comic c("f", "f", "f", 232, "f", Periodicity::WEEKLY, 2, 1, 2004);
    u + b;
    u.saveUserToBinaryFile("users.dat");
    l.readUsersFromBinaryFile("users.dat");
    l.addLiterature(b);
    l.addLiterature(c);
    l.printLibraryLiterature();
    return 0;
}