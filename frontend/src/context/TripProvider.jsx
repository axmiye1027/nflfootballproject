import React, { createContext, useState, useContext } from 'react';

const TripContext = createContext();

export function useTrip() {
  return useContext(TripContext);
}

function TripProvider({ children }) {
  const [tripResult, setTripResult] = useState({
    stadiums: [],
    totalDistance: 0,
  });

  const clearTrip = () => {
    setTripResult({ stadiums: [], totalDistance: 0 });
  };

  return (
    <TripContext.Provider value={{ tripResult, setTripResult, clearTrip }}>
      {children}
    </TripContext.Provider>
  );
}

export default TripProvider