// homepage / home button - first page to appear when opened

import { useEffect, useState } from 'react'
import { BrowserRouter, Link, Route, Routes } from 'react-router-dom'
import './styles/App.css'

// components
import NavBar from './components/NavBar.jsx'
import SearchBar from './components/SearchBar.jsx'
import TeamInfo from './components/TeamInfo.jsx'
// data
//import teams from './data/teams.json'
import LoginPage from "./pages/login.jsx"

function App() {
  const [count, setCount] = useState(0);
  const [stadiums, setStadiums] = useState([]);
  const [division, setDivision] = useState("All Teams");

  const [search,      setSearch]      = useState("");
  const [searchInput, setSearchInput] = useState("");

  // return (
  //   <BrowserRouter>
  //     <Link to="/login"></Link>
  //     <Routes>
  //        <Route path="/" element={<h1>BADADAD</h1>} />
  //       <Route path="/login" element={<LoginPage />} />
  //     </Routes> 
  //   </BrowserRouter>
  // )

  // Debounce: wait 400ms AFTER user stops typing
  useEffect(() => {
    const timeout = setTimeout(() => {
      setSearch(searchInput);
    }, 400);

    return () => clearTimeout(timeout);
  }, [searchInput]);


  useEffect(() => {
    const url =
      division === "All Teams"
        ? `http://localhost:18080/stadiums?search=${search}`
        : `http://localhost:18080/stadiums?division=${division}&search=${search}`;

    fetch(url)
      .then((res) => res.json())
      .then((data) => {
        setStadiums(data.stadiums || []);
      })
      .catch((err) => {
        console.error(err);
      });
  }, [division, search]);

  return (
    <BrowserRouter>
      <>
        {/* NAVIGATION */}
        <NavBar />

        {/* ROUTES */}
        <Routes>
            <Route path="/login" element={<LoginPage />} />
        </Routes> 


        {/* HEADER */}
        <div className="title" style={{ 
          marginTop: '1em', 
          backgroundSize: 'cover',
          backgroundPosition: 'center',
          backgroundImage: `url('')` }}>List of Teams</div>

        {/* SEARCH - ICON - FILTERS */}
        <div className="row">

          {/* SEARCH BAR */}
          <SearchBar 
            searchInput={searchInput} 
            setSearchInput={setSearchInput} 
          />

          {/* LOGO */}

          {/* FILTERS */}
          <select
            value={division}
            onChange={(e) => setDivision(e.target.value)}
          >
            <option value="All Teams">All Teams</option>
            <option value="AFC">AFC</option>
            <option value="NFC">NFC</option>
          </select>

        </div>
        {/* END SEARCH - ICON - FILTERS */}

        {/* TEAM LIST */}
        <div className="teams-container">
          <div 
            style={{ 
              display: "flex", 
              flexWrap: "wrap", 
              gap: "20px", 
              justifyContent: "center",
              marginTop: "40px",
            }}
          >
            {stadiums.map((stadium) => (
              <TeamInfo key={stadium.stadiumId} stadium={stadium} />
            ))}
          </div>
        </div>
        {/* END TEAM LIST */}
  {/* 
        <Routes>
          <Route path="/" element={<h1>BADADAD</h1>} />
          <Route path="/login" element={<LoginPage />} />
        </Routes> */}
      </>
    </BrowserRouter>
  )
}

export default App
