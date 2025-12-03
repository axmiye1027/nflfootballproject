import { useState, useEffect } from 'react'
import { useLocation } from "react-router-dom";

import SouvenirCard from '../components/SouvenirCard';

import '../styles/summary.css'

export default function SummaryPage() {
	const location = useLocation();
	const { stadiums, totalDistance } = location.state || {};
	const [ fullStadiums, setFullStadiums ] = useState([]);
	const [ cartTotal, setCartTotal] = useState(0);
	const [totals, setTotals] = useState({}); // {stadiumId: subtotal}

	useEffect(() => {
        if (!stadiums) return;

        async function fetchStadiumDetails() {
            try {
                const res = await fetch("http://localhost:18080/souvenirs", {
                    method: "POST",
                    headers: { "Content-Type": "application/json" },
                    body: JSON.stringify({ stadiums: normalizeStadiumList(stadiums) })
                });

				const data = await res.json();
				console.log("Backend data:", data);

				setFullStadiums(data.stadiums ?? []);
            } catch (err) {
                console.error("Failed to fetch stadium details:", err);
            }
        }

        fetchStadiumDetails();
    }, [stadiums]);

	function normalizeStadiumList(stadiums) {
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

			<div className="row">

				{ /* LEFT SIDE - TEAMS */ }
				<div className="teamRow">

					{ /* TEAM CONTAINER*/ }
					<div className="teamContainer">

						{ /* SOUVENIERS */ }
						<div className="souveniersContainer">

							<div className="row">
								<div className="fullStadiums-map">
									{fullStadiums.map((stadium) => (
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