#include <sqlite3.h>
#include <iostream>

//add manga to database
void addManga( sqlite3* db, std::string title, std::string author, std::string genre, int vols ) { // takes in arguments: database, title, author, genre, volumes
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO MANGA (title, author, genre, volumes) VALUES (?, ?, ?, ?)"; // sql statement

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); // prepare statement
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    
    sqlite3_bind_text ( stmt, 1, title.c_str(), -1, SQLITE_STATIC ); // bind title = send title to database
    sqlite3_bind_text ( stmt, 2, author.c_str(), -1, SQLITE_STATIC ); // bind author = send author to database
    sqlite3_bind_text ( stmt, 3, genre.c_str(), -1, SQLITE_STATIC ); // bind genre = send genre to database
    sqlite3_bind_int  ( stmt, 4, vols ); // bind vols = send volumes to database
    rc = sqlite3_step ( stmt );
    if ( rc != SQLITE_DONE) {
        std::cerr << "Error adding manga: " << sqlite3_errmsg ( db ) << std::endl;
    } else {
        std::cout << "Manga added successfully" << std::endl;
    }
    sqlite3_finalize( stmt ); // close Statement 

}
 
void removeManga (sqlite3* db, const std::string& mangaTitle) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM MANGA WHERE title = ?";


    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    } else {
        std::cout << "Manga removed successfully" << std::endl;
    }

    // Bind the manga title to the first placeholder in the SQL statement
    rc = sqlite3_bind_text (stmt, 1, mangaTitle.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding title: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    // Execute the SQL statement
    rc = sqlite3_step( stmt);
    if ( rc != SQLITE_DONE ) {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Manga removed successfully" << std::endl;
    }

    // Finalize the statement to free the memory
    sqlite3_finalize( stmt );
}

void viewManga ( sqlite3* db ) {
    sqlite3_stmt* stmt;
    std::string sql = "SELECT * FROM MANGA";
    int rc = sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, nullptr );
    if ( rc != SQLITE_OK ) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg( db ) << std::endl;
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::cout << "------------------------------" << std::endl;
        std::cout << "Title: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) << std::endl;
        std::cout << "Author: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) << std::endl;
        std::cout << "Genre: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) << std::endl;
        std::cout << "Volumes: " << sqlite3_column_int(stmt, 3) << std::endl;
        std::cout << "------------------------------" << std::endl;
    }

    sqlite3_finalize(stmt);
}

int main() {

    //connect database
    sqlite3* db;
    char* errMessage = nullptr;
    const char* dbPath = "MangaLibrary.db"; // pointers to database
    int rc = sqlite3_open(dbPath, &db); // open database
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    } else {
        std::cout << "Connected to database successfully" << std::endl;
    }

    //user register & logins
    int choice;
    std::string title;
    std::string author;
    std::string genre;
    int vols;
    

    //user register & logins
    std::cout << "***************** MANGA LIBRARY *****************" << std::endl;
    std::cout << "1. Add Manga" << std::endl;
    std::cout << "2. Remove Manga" << std::endl;
    std::cout << "3. View Manga" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter 1, 2, 3 or 4: ";

    while ( choice != 4){
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch ( choice ) {
                case 1:
                    std::cout << "Title: ";
                    std::getline(std::cin, title);
                    std::cout << "Author: ";
                    std::getline(std::cin, author);
                    std::cout << "Genre: ";
                    std::getline(std::cin, genre);
                    std::cout << "Volumes: ";
                    std::cin >> vols;
                    addManga(db, title, author, genre, vols);
                    break;
                case 2:
                    std::cout << "Title: ";
                    std::getline(std::cin, title);
                    removeManga(db, title);
                    break;
                case 3:
                    viewManga(db);
                    break;
                case 4:
                    std::cout << "Exiting..." << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice" << std::endl;
            }
            std::cout << "1. Add Manga" << std::endl;
            std::cout << "2. Remove Manga" << std::endl;
            std::cout << "3. View Manga" << std::endl;
            std::cout << "4. Exit" << std::endl;
            std::cout << "Enter 1, 2, 3 or 4: ";
    }

    return 0;
}