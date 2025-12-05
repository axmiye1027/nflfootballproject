import { useState, useEffect } from 'react'
import { useNavigate } from "react-router-dom";

import { useTrip } from '../context/TripProvider.jsx';

import SouvenirCard from '../components/SouvenirCard';

import '../styles/summary.css'

export default function SummaryPage() {
	const navigate = useNavigate();
	const { tripResult, clearTrip } = useTrip();
	const { stadiums = [], totalDistance = 0 } = tripResult || {};

	const [totals, setTotals] = useState({});
	
	// useEffect(() => {
	// 	if (!stadiums || stadiums.length === 0) 
	// 	{
	// 		setFullStadiums([]);
	// 		return;
	// 	}

	// 	async function fetchStadiumDetails() 
	// 	{
	// 		try {
	// 			const res = await fetch("http://localhost:18080/souvenirs", {
	// 				method: "POST",
	// 				headers: { "Content-Type": "application/json" },
	// 				body: JSON.stringify({ stadiums: normalizeStadiumList(stadiums) })
	// 			});

	// 			const data = await res.json();

	// 			const ordered = normalizeStadiumList(stadiums).map(
	// 				name => data.stadiums.find(s => s.stadiumName === name)
	// 			);

	// 			setFullStadiums(ordered);
	// 		} 
	// 		catch (err) 
	// 		{
	// 			console.error("Failed to fetch stadium details:", err);
	// 		}
	// 	}

	// 	fetchStadiumDetails();
	// }, [stadiums]);

	function normalizeStadiumList(stadiums) 
	{
		if (!stadiums) return [];

		if (typeof stadiums === "string") return [stadiums];

		if (Array.isArray(stadiums)) {
			if (stadiums.length === 0) return [];
			if (typeof stadiums[0] === "string") return stadiums;
			if (typeof stadiums[0] === "object") return stadiums.map(s => s.stadiumName);
		}

		return [];
	}

	const handleQuantityChange = (stadiumId, subtotal) => {
		setTotals(prev => ({ ...prev, [stadiumId]: subtotal }));
	};

	const grandTotal = Object.values(totals).reduce((a, b) => a + b, 0);

	return (
		<div className="summary-page">
			{ /* Header */ }
			<div className="title">Summary</div>
			<div>
				<button onClick={() => { clearTrip(); navigate("/trip"); }}>
					Start Over
				</button>
			</div>

			<div className="row">

				{ /* LEFT SIDE - TEAMS */ }
				<div className="teamRow">

					{ /* TEAM CONTAINER*/ }
					<div className="teamContainer">

						{ /* SOUVENIERS */ }
						<div className="souveniersContainer">

							<div className="row">
								<div className="stadiums-map">
									{stadiums.map((stadium) => (
										<SouvenirCard 
											key={stadium.stadiumId} 
											stadium={stadium}
											onSubtotalChange={(subtotal) => handleQuantityChange(stadium.stadiumId, subtotal)}
										/>
									))}
								</div>
							</div>

						</div>
						{ /* END SOUVENIERS */}

					</div>
					{ /* END TEAM CONTAINER*/ }

				</div>
				{ /* END LEFT SIDE - TEAMS */ }


				{ /* RIGHT SIDE - TOTALS */ }
				<div  className="totalContainer">

					{ /* STADIUM*/ }
					<div>

						<div className="row">
							<div className="importantText">Cart:</div>
						</div>

					</div>
					{ /* END STADIUM*/ }


					{ /* SUMMARY */ }
					<div className="summaryContainer">
						{ /* STADIUM */ }
						<div className="row">
							<div className="total-header">Total:</div>
							<div className="total">${grandTotal.toFixed(2)}</div>
						</div>

						{ /* DISTANCE */ }
						<div className="row">
							<div className="totalDistance-header">Total Distance:</div>
							<div className="totalDistance">{totalDistance} mi</div>
						</div>
					</div>
					{ /* END SUMMARY */ }

				</div>
				{ /* END RIGHT SIDE - TOTAL */ }

			</div>
		</div>
  );
}