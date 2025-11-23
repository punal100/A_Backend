# A_Backend_Server

This project is a C++ backend server for A_ServerHost with a React frontend, with CMake as Build system and preferably MSVC as Compiler, It provides basic user management (registration, login, listing) and player data retrieval.

## Technologies Used

### Backend

- **C++20**
- **[Crow](https://github.com/CrowCpp/Crow)**: A fast and easy-to-use C++ micro web framework.
- **[libpqxx](https://github.com/jtv/libpqxx)**: The official C++ client API for PostgreSQL.
- **[CMake](https://cmake.org/)**: For building the backend application.
- **[vcpkg](https://github.com/microsoft/vcpkg)**: For C++ library management.

### Frontend

- **[React](https://reactjs.org/)**: A JavaScript library for building user interfaces.
- **[Axios](https://github.com/axios/axios)**: For making API requests from the frontend.

## Features

- User registration and login.
- Session management returning a token and user roles.
- Display a list of registered users.
- Fetch and display player-specific data, including level, XP, and inventory.
- A simple web interface to interact with the backend services.

## API Endpoints and Usage

The backend exposes the following RESTful endpoints:

### User Management

#### `POST /register`

Registers a new user.

- **Request Body**:
  ```json
  {
    "username": "newuser",
    "password": "password123"
  }
  ```
- **Success Response (201 Created)**:
  ```json
  {
    "message": "User registered successfully"
  }
  ```
- **Example `curl`**:
  ```bash
  curl -X POST -H "Content-Type: application/json" -d '{"username":"newuser","password":"password123"}' http://localhost:18080/register
  ```

#### `POST /login`

Logs in a user and returns a session token and roles.

- **Request Body**:
  ```json
  {
    "username": "newuser",
    "password": "password123"
  }
  ```
- **Success Response (200 OK)**:
  ```json
  {
    "token": "some-jwt-or-session-token",
    "roles": ["player"]
  }
  ```
- **Example `curl`**:
  ```bash
  curl -X POST -H "Content-Type: application/json" -d '{"username":"newuser","password":"password123"}' http://localhost:18080/login
  ```

#### `GET /users`

Retrieves a list of all registered users.

- **Success Response (200 OK)**:
  ```json
  [
    { "id": 1, "username": "admin", "roles": ["admin", "player"] },
    { "id": 2, "username": "newuser", "roles": ["player"] }
  ]
  ```
- **Example `curl`**:
  ```bash
  curl http://localhost:18080/users
  ```

## Project Structure

```
A_Backend/
├── CMakeLists.txt          # Main CMake build script
├── CMakePresets.json       # CMake presets for Visual Studio
├── vcpkg.json              # vcpkg dependencies
├── src/
│   ├── backend/            # (Assumed) C++ source files for the server
│   └── frontend/           # React frontend application
│       ├── public/
│       └── src/
│           ├── api/
│           │   └── axios.js
│           └── components/
│               ├── LoginUser.jsx
│               ├── PlayerDetails.jsx
│               ├── RegisterUser.jsx
│               └── UserList.jsx
└── README.md               # This file
```

## Getting Started

### Prerequisites

- A C++20 compatible compiler (e.g., MSVC, GCC, Clang).
- CMake (version 3.15+).
- `vcpkg` for dependency management.
- Node.js and npm for the frontend.

### Backend Setup

1.  **Install Dependencies**: Run `vcpkg install` to get `crow` and `libpqxx`.
2.  **Configure CMake**: Use `CMakePresets.json` with a compatible generator like Visual Studio 2022.
    ```bash
    cmake --preset=x64-debug
    ```
3.  **Build**: Build the project using CMake.
    ```bash
    cmake --build --preset=x64-debug
    ```

### Frontend Setup

1.  Navigate to the frontend directory: `cd src/frontend`
2.  Install dependencies: `npm install`
3.  Start the development server: `npm start`
