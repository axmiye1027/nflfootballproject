// homepage / home button - first page to appear when opened

import { useEffect, useState } from 'react'
//import { BrowserRouter, Link, Route, Routes } from 'react-router-dom'
import './styles/App.css'

// components
import NavBar from './components/NavBar.jsx'
import SearchBar from './components/SearchBar.jsx'
import TeamInfo from './components/TeamInfo.jsx'
// data
//import teams from './data/teams.json'
//import LoginPage from "./pages/login.jsx"

function App() {
  const [count, setCount] = useState(0);
  const [stadiums, setStadiums] = useState([]);
  const [loading,  setLoading]  = useState(true);

  // return (
  //   <BrowserRouter>
  //     <Link to="/login"></Link>
  //     <Routes>
  //        <Route path="/" element={<h1>BADADAD</h1>} />
  //       <Route path="/login" element={<LoginPage />} />
  //     </Routes> 
  //   </BrowserRouter>
  // )

  useEffect(() => {
    fetch("http://localhost:18080/stadiums")
    .then((res)  => res.json())
    .then((data) => {
      setStadiums(data.stadiums);
      setLoading(false);
    })
    .catch((err) => {
      console.error("Error fetching stadiums:", err);
      setLoading(false);
    });
  }, []);

  if (loading) return <p>Loading stadiums...</p>;

  return (
    <>
      {/* NAVIGATION */}
      <NavBar />

      {/* HEADER */}
      <div className="title" style={{ 
        backgroundSize: 'cover',
        backgroundPosition: 'center',
        backgroundImage: `url('')` }}>List of Teams</div>

      {/* SEARCH - ICON - FILTERS */}
      <div className="row">

        {/* SEARCH BAR */}
        <SearchBar />

        {/* LOGO */}

        {/* FILTERS */}
        <select>
          <option value="">All Teams</option>
          <option value="AFC">AFC</option>
          <option value="NFC">NFC</option>
        </select>

      </div>
      {/* END SEARCH - ICON - FILTERS */}

      <div> 
        <h1>Stadiums</h1>
        <ul style={{ listStyle: "none", padding: 0}}>
          {stadiums.map((s) => (
            <li
              key={s.stadiumId}
              style={{
                padding: "12px",
                maginBottom: "10px",
                border: "1px solid #ccc",
                borderRadius: "8px"
              }}
            >
              <h3>{s.stadiumName}</h3>
              <p><b>Team:    </b> {s.teamName}</p>
              <p><b>Stadium: </b> {s.stadiumName}</p>
              <p><b>Location:</b> {s.location}</p>
              <p><b>Capacity:</b> {s.capacity.toLocaleString()}</p>

            </li>
          ))}
        </ul>
      </div>

      {/* TEAM LIST */}
      <TeamInfo />
      {/* END TEAM LIST */}
{/* 
      <Routes>
         <Route path="/" element={<h1>BADADAD</h1>} />
        <Route path="/login" element={<LoginPage />} />
      </Routes> */}
    </>
  )
}

export default App
