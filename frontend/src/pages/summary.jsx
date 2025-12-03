import React, { useMemo, useState, useEffect } from 'react'

import '../styles/summary.css'

export default function SummaryPage() {

	return (
		<div>
			{ /* Header */ }
			<div className="title">Summary</div>

			<div className="row">

				{ /* LEFT SIDE - TEAMS */ }
				<div className="teamRow">

					{ /* TEAM CONTAINER*/ }
					<div className="teamContainer">
						<div className="header">LOREM_TEAM</div>
						<div className="subHead">LOREM_STADIUM</div>

						{ /* SOUVENIERS */ }
						<div className="souveniersContainer">

							<div className="row">
								<div className="importantText">LOREM_ITEM</div>
								<div>$ LOREM_PRICE</div>

								{ /* USER INPUT NUMBER */}
								<input className="quantityContainer"
									type="number"
									min={0} />

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
							<div className="importantText">LOREM_STADIUM:</div>
							<div>$ LOREM</div>
						</div>

					</div>
					{ /* END STADIUM*/ }


					{ /* SUMMARY */ }
					<div className="summaryContainer">
						{ /* STADIUM */ }
						<div className="row">
							<div className="importantText">Total:</div>
							<div>$ LOREM</div>
						</div>

						{ /* DISTANCE */ }
						<div className="row">
							<div className="importantText">Total Distance:</div>
							<div>LOREM</div>
						</div>
					</div>
					{ /* END SUMMARY */ }

				</div>
				{ /* END RIGHT SIDE - TOTAL */ }

			</div>
		</div>
  );
}