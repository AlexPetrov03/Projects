// A functioning Blockchain written in C++. It was before we learned how classes function so it only uses structures.
#include <iostream>
#include <chrono>
#include <cstring>
#include <fstream>
const int SYSTEM_USER_ID = 0;

struct User
{
  unsigned id;
  char name[128];
};
struct Transaction
{
  unsigned sender;
  unsigned receiver;
  double coins;
  long long time;
};

struct TransactionBlock
{
  unsigned id;
  unsigned prevBlockId;
  unsigned prevBlockHash;
  int validTransactions;
  Transaction transactions[16];
};
unsigned computeHash(const unsigned char *memory, int length)
{
  unsigned hash = 0xbeaf;

  for (int c = 0; c < length; c++)
  {
    hash += memory[c];
    hash += hash << 10;
    hash ^= hash >> 6;
  }

  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}
time_t timeSinceEpoch() // source: https://stackoverflow.com/questions/14315497/how-to-get-current-date-time-of-the-system-in-seconds
{
  auto now = std::chrono::system_clock::now();
  return std::chrono::system_clock::to_time_t(now);
}
TransactionBlock getLastTransactionBlock(std::fstream &blocks)
{
  blocks.seekg(0, std::ios::beg);
  TransactionBlock buff;
  while (blocks.read(reinterpret_cast<char *>(&buff), sizeof(TransactionBlock)))
  {
  }
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  return buff;
}
bool saveInFile(std::fstream &file, const User &user)
{
  file.clear();
  file.seekp(0, std::ios::end);
  if (!file.is_open())
  {
    std::cout << "The file could not be opened!" << std::endl;
    return false;
  }
  file.write(reinterpret_cast<const char *>(&user), sizeof(User));
  file.seekp(0, std::ios::beg);
  return true;
}
bool saveInFile(std::fstream &file, const TransactionBlock &tb)
{
  file.seekp(0, std::ios::end);
  if (!file.is_open())
  {
    std::cout << "The file could not be opened!" << std::endl;
    return false;
  }
  file.write(reinterpret_cast<const char *>(&tb), sizeof(TransactionBlock));
  file.seekp(0, std::ios::beg);
  return true;
}
bool editInFile(std::fstream &file, const TransactionBlock &tb)
{
  std::streampos pos = -1;
  if (!file.is_open())
  {
    std::cout << "The file could not be opened!" << std::endl;
    return false;
  }
  file.seekg(0, std::ios::beg);
  TransactionBlock buffer;
  while (file.read(reinterpret_cast<char *>(&buffer), sizeof(TransactionBlock)))
  {
    if (tb.id == buffer.id)
    {
      pos = static_cast<std::streampos>(file.tellg()) - static_cast<std::streampos>(sizeof(TransactionBlock));
      break;
    }
  }
  if (pos == -1)
  {
    return false;
  }
  file.seekp(pos, std::ios::beg);
  if (!file.write(reinterpret_cast<const char *>(&tb), sizeof(TransactionBlock)))
  {
    file.seekp(0, std::ios::beg);
    return false;
  }
  file.seekp(0, std::ios::beg);
  return true;
}
double getUserBalanceById(const unsigned int id, std::fstream &blocks)
{
  if (id == SYSTEM_USER_ID)
    return std::numeric_limits<double>::infinity(); // https://en.cppreference.com/w/cpp/types/numeric_limits/infinity
  double balance = 0;
  TransactionBlock buffer;
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  while (blocks.read(reinterpret_cast<char *>(&buffer), sizeof(TransactionBlock)))
  {
    for (std::size_t i = 0; i < buffer.validTransactions; i++)
    {
      if (buffer.transactions[i].receiver == id)
        balance += buffer.transactions[i].coins;
      else if (buffer.transactions[i].sender == id)
        balance -= buffer.transactions[i].coins;
    }
    if (blocks.fail())
    {
      std::cout << "There was an error reading the file." << std::endl;
    }
  }
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  return balance;
}
bool createTransaction(std::fstream &blocks, Transaction &transaction, const unsigned sender, const unsigned receiver, const double coins)
{
  if (getUserBalanceById(sender, blocks) < coins || sender == receiver)
    return false;
  transaction.sender = sender;
  transaction.receiver = receiver;
  transaction.coins = coins;
  transaction.time = static_cast<long long>(timeSinceEpoch());
  return true;
}
bool existsInFile(std::fstream &blocks, const int id)
{
  blocks.seekg(0, std::ios::end);
  if (blocks.tellg() == 0)
    return false;
  blocks.seekg(0, std::ios::beg);
  TransactionBlock buffer;
  while (blocks.read(reinterpret_cast<char *>(&buffer), sizeof(TransactionBlock)))
  {
    if (buffer.id == id)
    {
      blocks.clear();
      blocks.seekg(0, std::ios::beg);
      return true;
    }
  }
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  return false;
}
void createFirstTransactionBlock(TransactionBlock &tb)
{
  tb.id = 1;
  tb.prevBlockId = 1;
  tb.validTransactions = 0;
}
void createTransactionBlock(TransactionBlock &tb, const TransactionBlock &previous)
{
  tb.prevBlockId = previous.id;
  tb.id = previous.id + 1;
  tb.prevBlockHash = computeHash(reinterpret_cast<const unsigned char *>(&previous), sizeof(TransactionBlock));
  tb.validTransactions = 0;
}
void addTransactionToBlock(const Transaction &transaction, TransactionBlock &tb, std::fstream &blocks)
{
  if (tb.validTransactions == 16)
  {
    TransactionBlock newTb;
    createTransactionBlock(newTb, getLastTransactionBlock(blocks));
    newTb.validTransactions++;
    newTb.transactions[newTb.validTransactions - 1].coins = transaction.coins;
    newTb.transactions[newTb.validTransactions - 1].receiver = transaction.receiver;
    newTb.transactions[newTb.validTransactions - 1].sender = transaction.sender;
    newTb.transactions[newTb.validTransactions - 1].time = transaction.time;
    saveInFile(blocks, newTb);
  }
  else
  {
    tb.validTransactions++;
    tb.transactions[tb.validTransactions - 1].coins = transaction.coins;
    tb.transactions[tb.validTransactions - 1].receiver = transaction.receiver;
    tb.transactions[tb.validTransactions - 1].sender = transaction.sender;
    tb.transactions[tb.validTransactions - 1].time = transaction.time;
    if (existsInFile(blocks, tb.id))
    {
      editInFile(blocks, tb);
    }
    else
    {
      saveInFile(blocks, tb);
    }
  }
}
void createSystemUser(User &systemUser)
{
  systemUser.id = SYSTEM_USER_ID;
  strcpy(systemUser.name, "system-user");
}
bool userExistsInFile(std::fstream &users, const int id)
{
  users.clear();
  users.seekg(0, std::ios::beg);
  User buffer;
  while (users.read(reinterpret_cast<char *>(&buffer), sizeof(User)))
  {
    if (buffer.id == id)
    {
      users.seekg(0, std::ios::beg);
      users.clear();
      return true;
    }
  }
  users.clear();
  users.seekg(0, std::ios::beg);
  return false;
}
unsigned int getLastUserId(std::fstream &users)
{
  users.clear();
  users.seekg(0, std::ios::beg);
  User buffer;
  if (!users.read(reinterpret_cast<char *>(&buffer), sizeof(User)))
    return 0;
  while (users.read(reinterpret_cast<char *>(&buffer), sizeof(User)))
  {
  }
  users.clear();
  users.seekg(0, std::ios::beg);
  return buffer.id;
}
bool createUser(User &user, User &systemUser, const char name[128], const double money, std::fstream &users, std::fstream &blocks)
{
  if(!users.is_open()) return false;
  if(!blocks.is_open()) return false;
  unsigned int id = getLastUserId(users) + 1;
  if (userExistsInFile(users, id))
  {
    std::cout << "There is already a user with this id." << std::endl;
    return false;
  }
  user.id = id;
  strcpy(user.name, name);
  saveInFile(users, user);
  Transaction transaction;
  double coins = 420 * money;
  createTransaction(blocks, transaction, systemUser.id, user.id, coins);
  if (existsInFile(blocks, 1))
  {
    TransactionBlock last = getLastTransactionBlock(blocks);
    addTransactionToBlock(transaction, last, blocks);
  }
  else
  {
    TransactionBlock tb;
    createFirstTransactionBlock(tb);
    addTransactionToBlock(transaction, tb, blocks);
  }
  return true;
}
int countTransactionBlocks(std::fstream &blocks)
{
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  TransactionBlock buffer;
  int count = 0;
  while (blocks.read(reinterpret_cast<char *>(&buffer), sizeof(TransactionBlock)))
  {
    count++;
  }
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  return count;
}
int getUsersCount(std::fstream &users)
{
  users.clear();
  users.seekg(0, std::ios::beg);
  User buffer;
  int count = 0;
  while (users.read(reinterpret_cast<char *>(&buffer), sizeof(User)))
  {
    count++;
  }
  users.clear();
  users.seekg(0, std::ios::beg);
  return count;
}
bool wealthiestUsers(std::fstream &users, std::fstream &blocks, int COUNT)
{
  if (!users.is_open())
    return false;
  if (!blocks.is_open())
    return false;
  if (COUNT > getUsersCount(users))
  {
    wealthiestUsers(users, blocks, getUsersCount(users));
    return true;
  }
  users.clear();
  users.seekg(0, std::ios::beg);
  User currentUser;
  User wealthy[COUNT];
  int userIds[COUNT];
  double maxBalances[COUNT] = {0};
  while (users.read(reinterpret_cast<char *>(&currentUser), sizeof(User)))
  {
    double balance = getUserBalanceById(currentUser.id, blocks);
    for (int i = 0; i < COUNT; i++)
    {
      if (balance > maxBalances[i])
      {
        for (int j = COUNT - 1; j > i; j--)
        {
          wealthy[j] = wealthy[j - 1];
          maxBalances[j] = maxBalances[j - 1];
        }
        wealthy[i] = currentUser;
        maxBalances[i] = balance;
        break;
      }
    }
    if (users.fail())
      return false;
  }
  char *fileName = new char[128];
  strcpy(fileName, "wealthiest-users");
  time_t time = timeSinceEpoch();
  char timeStr[20];
  sprintf(timeStr, "%ld", time); //https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm
  strcat(fileName, timeStr);
  strcat(fileName, ".txt");
  std::ofstream wealthiestUsers(fileName, std::ios::out);
  if (!wealthiestUsers.is_open())
    return false;
  delete[] fileName;
  for (int i = 0; i < COUNT; i++)
  {
    wealthiestUsers << wealthy[i].name << " " << getUserBalanceById(wealthy[i].id, blocks) << std::endl;
  }
  users.clear();
  users.seekg(0, std::ios::beg);
  wealthiestUsers.close();
  return true;
}
double getBlockBalanceById(const int id, std::fstream &blocks)
{
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  TransactionBlock buffer;
  double balance = 0;
  while (blocks.read(reinterpret_cast<char *>(&buffer), sizeof(TransactionBlock)) && !blocks.eof())
  {
    if (buffer.id == id)
    {
      for (std::size_t i = 0; i < buffer.validTransactions; i++)
      {
        balance += buffer.transactions[i].coins;
      }
    }
  }
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  return balance;
}
TransactionBlock getBlockById(const int id, std::fstream &blocks)
{ blocks.clear();
  blocks.seekg(0, std::ios::beg);
  TransactionBlock buffer;
  while (blocks.read(reinterpret_cast<char *>(&buffer), sizeof(TransactionBlock)))
  {
    if (buffer.id == id)
    {
      return buffer;
    }
  }
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
}
bool biggestBlocks(std::fstream &blocks, const int COUNT)
{
  if (!blocks.is_open())
    return false;
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  if(COUNT > getLastTransactionBlock(blocks).id){
    biggestBlocks(blocks, getLastTransactionBlock(blocks).id);
    return true;
  }
  TransactionBlock currentBlock;
  TransactionBlock biggest[COUNT];
  double maxBalances[COUNT] = {0};
  while (blocks.read(reinterpret_cast<char *>(&currentBlock), sizeof(TransactionBlock)))
  {
    unsigned int currentPos = blocks.tellg();
    double balance = getBlockBalanceById(currentBlock.id, blocks);
    blocks.seekg(currentPos, std::ios::beg);
    for (int i = 0; i < COUNT; i++)
    {
      if (balance > maxBalances[i])
      {
        for (int j = COUNT - 1; j > i; j--)
        {
          biggest[j] = biggest[j - 1];
          maxBalances[j] = maxBalances[j - 1];
        }
        biggest[i] = currentBlock;
        maxBalances[i] = balance;
        break;
      }
    }
  }
  if(blocks.fail()) return false;
  char *fileName = new char[128];
  strcpy(fileName, "biggest-blocks");
  time_t time = timeSinceEpoch();
  char timeStr[20];
  sprintf(timeStr, "%ld", time);
  strcat(fileName, timeStr);
  strcat(fileName, ".txt");
  std::ofstream biggestBlocks(fileName, std::ios::out);
  if (!biggestBlocks.is_open())
    return false;
  delete[] fileName;
  for (int i = 0; i < COUNT; i++)
  {
    biggestBlocks << biggest[i].id << " " << getBlockBalanceById(biggest[i].id, blocks) << std::endl;
  }
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  biggestBlocks.close();
  return true;
}
bool verifyTransactions(std::fstream &blocks)
{
  if(!blocks.is_open()) return false;
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  TransactionBlock buffer;
  while (blocks.read(reinterpret_cast<char *>(&buffer), sizeof(TransactionBlock)))
  {
    if (buffer.validTransactions == 0)
    {
      blocks.clear();
      blocks.seekg(0, std::ios::beg);
      return false;
    }
    if (buffer.id != 1)
    {
      unsigned int pos = blocks.tellg();
      TransactionBlock previous = getBlockById(buffer.id - 1, blocks);
      blocks.seekg(pos, std::ios::beg);
      if (buffer.prevBlockHash != computeHash(reinterpret_cast<const unsigned char *>(&previous), sizeof(TransactionBlock)) || buffer.prevBlockId != previous.id)
      {
        return false;
      }
    }
  }
  blocks.clear();
  blocks.seekg(0, std::ios::beg);
  return true;
}
bool removeUser(std::fstream &users, std::fstream &blocks, const int userId)
{
  if (!users.is_open())
    return false;
  if(!blocks.is_open()) return false;
  users.clear();
  users.seekg(0, std::ios::beg);
  blocks.clear();
  blocks.seekg(0, std::ios::beg);

  std::fstream tempFile("temp.dat", std::ios::out | std::ios::binary);
  User currentUser;
  currentUser.id = 0;
  while (users.read(reinterpret_cast<char *>(&currentUser), sizeof(User)))
  {
    if (currentUser.id == userId)
    {
      double userBalance = getUserBalanceById(userId, blocks);
      Transaction transaction;
      createTransaction(blocks, transaction, userId, SYSTEM_USER_ID, userBalance);
      TransactionBlock tb = getLastTransactionBlock(blocks);
      addTransactionToBlock(transaction, tb, blocks);
    }
    else
    {
      tempFile.write(reinterpret_cast<char *>(&currentUser), sizeof(User));
    }
    if (users.fail())
      return false;
  }
  if (currentUser.id == 0)
    return false;
  users.close();
  tempFile.close();
  std::remove("users.dat");
  std::rename("temp.dat", "users.dat");
  users.open("users.dat", std::ios::in | std::ios::out | std::ios::binary);
  users.clear();
  users.seekg(0, std::ios::beg);
  if (!users.is_open())
    return false;
  return true;
}
int main()
{
  std::fstream users("users.dat", std::ios::in | std::ios::out | std::ios::binary);
  std::fstream blocks("blocks.dat", std::ios::in | std::ios::out | std::ios::binary);
  if(!users.is_open()){
    std::cout << "Users couldn't be created/opened." << std::endl;
    return -1;
  }
  if(!blocks.is_open()){
    std::cout << "Blocks couldn't be created/opened." << std::endl;
    return -1;
  }
  User systemUser;
  createSystemUser(systemUser);
  std::cout << "Please enter one of the following commands:" << std::endl << " verify-transactions" << std::endl << " create-user"
  << std::endl << " remove-user" << std::endl << " send-coins" << std::endl << " wealthiest-users" << std::endl << " biggest-blocks" << std::endl;
  char input[32];
  std::cin.getline(input, 20);

  while (strcmp(input, "exit") != 0)
  {
    if (strcmp(input, "verify-transactions") == 0)
    {
      if (verifyTransactions(blocks))
        std::cout << "All of the transaction blocks are valid!" << std::endl;
      else
        std::cout << "The transaction block chain is not up to standart!" << std::endl;
    }
    else if (strcmp(input, "create-user") == 0)
    {
      std::cout << "Enter a username: ";
      char name[128];
      std::cin.getline(name, 128);
      double investment;
      std::cout << "Enter an amount to invest: ";
      std::cin >> investment;
      std::cin.ignore();
      User user;
      if (createUser(user, systemUser, name, investment, users, blocks))
      {
        std::cout << "User created successfully." << std::endl;
      }
      else
        std::cout << "User creation failed." << std::endl;
    }
    else if (strcmp(input, "remove-user") == 0)
    {
      std::cout << "Enter the id of the user you want to remove: ";
      unsigned int id;
      std::cin >> id;
      std::cin.ignore();
      if (removeUser(users, blocks, id))
      {
        std::cout << "User removed succcessfully." << std::endl;
      }
      else
        std::cout << "User removal failed." << std::endl;
    }
    else if (strcmp(input, "send-coins") == 0)
    {
      Transaction transaction;
      std::cout << "Enter the sender's id: ";
      unsigned int sender;
      std::cin >> sender;
      std::cin.ignore();
      std::cout << "Enter the receiver's id: ";
      unsigned int receiver;
      std::cin >> receiver;
      std::cin.ignore();
      std::cout << "Enter the coins for the transaction: ";
      double coins;
      std::cin >> coins;
      std::cin.ignore();
      if (createTransaction(blocks, transaction, sender, receiver, coins))
      {
        std::cout << "The transaction was successful." << std::endl;
        TransactionBlock tb = getLastTransactionBlock(blocks);
        addTransactionToBlock(transaction, tb, blocks);
      }
      else
        std::cout << "The transaction failed." << std::endl;
    }
    else if (strcmp(input, "wealthiest-users") == 0)
    {
      std::cout << "Enter how many of the wealthiest users you would like to see: ";
      int count;
      std::cin >> count;
      std::cin.ignore();
      if(wealthiestUsers(users, blocks, count)){
        std::cout << "The operation was successful." << std::endl;
      }else std::cout << "The operation failed." << std::endl;
    }
    else if (strcmp(input, "biggest-blocks") == 0)
    {
      std::cout << "Enter how many of the biggest blocks you would like to see: ";
      int count;
      std::cin >> count;
      std::cin.ignore();
      if(biggestBlocks(blocks, count)){
        std::cout << "The operation was successful." << std::endl;
      }else std::cout << "The operation failed." << std::endl;
    }
    else
    {
      std::cout << "Please, enter a valid command." << std::endl;
    }
    std::cin.getline(input, 20);
  }
  blocks.close();
  users.close();
}
