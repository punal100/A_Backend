import React from 'react';
import UserList from './components/UserList';
import RegisterUser from './components/RegisterUser';
import LoginUser from './components/LoginUser'; // IMPORT THIS
import PlayerDetails from './components/PlayerDetails';
import './App.css';

function App() {
    return (
        <div className="App">
            <header className="App-header">
                <h1>A_Backend Admin Panel</h1>
            </header>
            <main>
                <div className="component-container">
                    <RegisterUser />
                </div>
                
                {/* ADD THIS SECTION */}
                <div className="component-container">
                    <LoginUser />
                </div>

                <div className="component-container">
                    <PlayerDetails />
                </div>

                <div className="component-container">
                    <UserList />
                </div>
            </main>
        </div>
    );
}

export default App;