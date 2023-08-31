#include <iostream>
#include <cstring>
#include <chrono>
#include <fstream>
class Literature
{
private:
    char *title;
    char *description;
    unsigned int id;
    unsigned int year;
    void free();
    void copyFrom(const Literature &other);

public:
    Literature();
    Literature(const char *title, const char *description, unsigned int id, unsigned int year);
    Literature(const Literature &other);
    ~Literature();
    Literature &operator=(const Literature &other);
    void setTitle(const char *title);
    void setDescription(const char *description);
    void setId(unsigned int id);
    void setYear(unsigned int year);
    const char *getTitle() const;
    const char *getDescription() const;
    unsigned int getId() const;
    unsigned int getYear() const;
    virtual const char *getType() const = 0;

    void saveLiteratureToFile(const char* fileName);
};

class Book : public Literature
{
private:
    char *author;
    char *publishingHouse;
    unsigned int genre;
    void free();
    void copyFrom(const Book &other);

public:
    Book(const char *author, const char *title, const char *publishingHouse, unsigned int genre, const char *description, unsigned int id, unsigned int year);
    Book(const Book &other);
    Book();
    Book &operator=(const Book &other);
    ~Book();
    void setAuthor(const char *author);
    void setPublishingHouse(const char *publishingHouse);
    void setGenre(unsigned int genre);
    const char *getAuthor() const;
    const char *getPublishingHouse() const;
    unsigned int getGenre() const;
    const char *getType() const override;
    unsigned int getCharacteristics() const;
    unsigned int getGenreType() const;
    unsigned int getTargetAudience() const;
     unsigned int getEvenOdd() const;

    void setCharacteristics(unsigned int characteristics);
    void setType(unsigned int type);
    void setTargetAudience(unsigned int targetAudience);
    void setEvenOdd();
};
enum class Periodicity
{
    NOT_SET,
    WEEKLY,
    MONTHLY,
    YEARLY
};
class PeriodicalLiterature : public Literature
{
private:
    Periodicity periodicity;
    unsigned int issue;

    void copyFrom(const PeriodicalLiterature &other);

public:
    PeriodicalLiterature(const char *title, Periodicity periodicity, const char *description, unsigned int issue, unsigned int id, unsigned int year);
    PeriodicalLiterature(const PeriodicalLiterature &other);
    PeriodicalLiterature();
    PeriodicalLiterature &operator=(const PeriodicalLiterature &other);

    void setPeriodicity(Periodicity periodicity);
    void setIssue(unsigned int issue);

    Periodicity getPeriodicity() const;
    unsigned int getIssue() const;
    const char *getType() const override;
};

class Comic : public Literature
{
    private:
        char *author;
        char *publishingHouse;
        unsigned int genre;
        Periodicity periodicity;
        unsigned int issue;

        void free();
        void copyFrom(const Comic &other);
    public:
        Comic(const char* author, const char* title, const char* publishingHouse, unsigned int genre, const char* description, Periodicity periodicity, unsigned int issue, unsigned int id, unsigned int year);
        Comic();
        ~Comic();
        void setAuthor(const char *author);
        void setPublishingHouse(const char *publishingHouse);
        void setGenre(unsigned int genre);
        const char *getAuthor() const;
        const char *getPublishingHouse() const;
        unsigned int getGenre() const;
        const char *getType() const override;
        unsigned int getCharacteristics() const;
        unsigned int getGenreType() const;
        unsigned int getTargetAudience() const;
        unsigned int getEvenOdd() const;

        void setCharacteristics(unsigned int characteristics);
        void setType(unsigned int type);
        void setTargetAudience(unsigned int targetAudience);
        void setEvenOdd();
};

class User
{
private:
    char *name;
    unsigned int capacity;
    unsigned int *takenLiterature;
    unsigned int takenLiteratureSize;
    unsigned int *returnedLiterature;
    unsigned int returnedLiteratureSize;
    unsigned int numberOfTakenLiterature;

    void copyFrom(const User &other);
    void resizeLiterature();
    void takeLiterature(unsigned int id);
    void returnLiterature(unsigned int id);
    void free();

public:
    User(const char *name);
    User(const User &user);
    User();
    ~User();

    void setName(const char *name);
    void setCapacity(unsigned int capacity);
    void setTakenLiteratureSize(unsigned int takenLiteratureSize);
    void setReturnedLiteratureSize(unsigned int returnedLiteratureSize);
    void setNumberOfTakenLiterature(unsigned int numberOfTakenLiterature);

    bool hasTakenLiterature(unsigned int id);
    bool hasReturnedLiterature(unsigned int id);

    char *getName() const;
    unsigned int getCapacity() const;
    unsigned int getTakenLiterature(unsigned int id) const;
    unsigned int getTakenLiteratureSize() const;
    unsigned int getReturnedLiterature(unsigned int id) const;
    unsigned int getReturnedLiteratureSize() const;
    unsigned int getNumberOfTakenLiterature() const;

    User &operator=(const User &other);
    bool operator==(const User &other);
    bool operator!=(const User &other);
    bool operator<(const User &other);
    bool operator<=(const User &other);
    bool operator>(const User &other);
    bool operator>=(const User &other);
    User &operator+(const Literature &literature);
    User &operator+=(const Literature &literature);
    User &operator-(const Literature &literature);
    User &operator-=(const Literature &literature);
    int operator[](unsigned int id);

    void saveUserToBinaryFile(const char* fileName);
};

class Library
{
private:
    Literature **collection;
    unsigned int *takenLiterature;
    User **users;

    unsigned int size;
    unsigned int userSize;
    unsigned int takenLiteratureSize;
    unsigned int capacity;
    unsigned int userCapacity;

    void sortCollection();
    bool comparePeriodicals(const PeriodicalLiterature &per1, const PeriodicalLiterature &per2) const;
    void resizeCollection();
    void resizeUsers();
public:
    Library();
    ~Library();

    void addLiterature(const Literature &literature);
    void removeLiterature(unsigned int id);

    void addOrUpdateUser(const User &user);
    void removeUser(const char *name);
    void takeLiterature(User &user, unsigned int id);
    void returnLiterature(User& user, unsigned int id);
    bool userExists(const char* name)const;
    //bool isTaken(unsigned int id)const;

    Literature **getCollection() const;
    unsigned int getSize() const;
    unsigned int getCapacity() const;
    Literature &getLiteratureById(unsigned int id);
    unsigned int getUserIdByName(const char* name)const;

    void printLibraryLiterature() const;
    void printUsers()const;
    void printUsersHaveNotReturned(unsigned int id)const;
    void readLibraryFromBinaryFile(const char* fileName);
    void readUsersFromBinaryFile(const char* fileName);

    bool compareLiterature(const Literature &literature1, const Literature &literature2) const;
};