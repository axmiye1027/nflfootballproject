// homepage / home button - first page to appear when opened

import { useState } from 'react'
import { BrowserRouter, Route, Routes } from 'react-router-dom'

import './styles/App.css'

// components
import NavBar from './components/NavBar.jsx'
import Layout from "./components/Layout.jsx"

// data
//import teams from './data/teams.json'
import LoginPage   from "./pages/login.jsx"
import HomePage    from "./pages/home.jsx"
import AdminPage   from "./pages/admin.jsx"
import TripPage    from "./pages/trip.jsx"
import SummaryPage from "./pages/summary.jsx"

import TripProvider from './context/TripProvider.jsx';

function App() {
  const [loggedIn, setLoggedIn] = useState(false);
  const [stadiums, setStadiums] = useState([]);

  return (
    <BrowserRouter>
      <TripProvider> 
        <>
          {/* NAVIGATION */}
          {/* <NavBar loggedIn={loggedIn} /> */}

          {/* ROUTES */}
          <Routes>
            <Route element={<Layout loggedIn={loggedIn} />}>
              <Route path="/"        element={<HomePage  stadiums={stadiums} setStadiums={setStadiums}/>} />
              <Route path="/login"   element={<LoginPage setLoggedIn={setLoggedIn} />} />
              <Route path="/admin"   element={<AdminPage stadiums={stadiums} />} />
              <Route path="/trip"    element={<TripPage />} />
              <Route path="/summary" element={<SummaryPage />} />
            </Route>
          </Routes> 
        </>
      </TripProvider>
    </BrowserRouter>
  )
}

export default App
