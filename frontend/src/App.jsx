// homepage / home button - first page to appear when opened

import { useState } from 'react'
import { BrowserRouter, Route, Routes } from 'react-router-dom'
import './styles/App.css'

// components
import NavBar from './components/NavBar.jsx'

// data
//import teams from './data/teams.json'
import LoginPage from "./pages/login.jsx"
import HomePage  from "./pages/home.jsx"

function App() {
  const [loggedIn, setLoggedIn] = useState(false);

  return (
    <BrowserRouter>
      <>
        {/* NAVIGATION */}
        <NavBar loggedIn={loggedIn} />

        {/* ROUTES */}
        <Routes>
          <Route path="/" element={<HomePage />} />
          <Route path="/login" element={<LoginPage setLoggedIn={setLoggedIn} />} />
        </Routes> 
      </>
    </BrowserRouter>
  )
}

export default App
