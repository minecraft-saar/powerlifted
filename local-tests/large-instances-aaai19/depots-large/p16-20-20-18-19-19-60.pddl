(define (problem depotprob16) (:domain depots)
(:objects
	depot0 depot1 depot2 depot3 depot4 depot5 depot6 depot7 depot8 depot9 depot10 depot11 depot12 depot13 depot14 depot15 depot16 depot17 depot18 depot19 - Depot
	distributor0 distributor1 distributor2 distributor3 distributor4 distributor5 distributor6 distributor7 distributor8 distributor9 distributor10 distributor11 distributor12 distributor13 distributor14 distributor15 distributor16 distributor17 distributor18 distributor19 - Distributor
	truck0 truck1 truck2 truck3 truck4 truck5 truck6 truck7 truck8 truck9 truck10 truck11 truck12 truck13 truck14 truck15 truck16 truck17 - Truck
	pallet0 pallet1 pallet2 pallet3 pallet4 pallet5 pallet6 pallet7 pallet8 pallet9 pallet10 pallet11 pallet12 pallet13 pallet14 pallet15 pallet16 pallet17 pallet18 pallet19 pallet20 pallet21 pallet22 pallet23 pallet24 pallet25 pallet26 pallet27 pallet28 pallet29 pallet30 pallet31 pallet32 pallet33 pallet34 pallet35 pallet36 pallet37 pallet38 pallet39 - Pallet
	crate0 crate1 crate2 crate3 crate4 crate5 crate6 crate7 crate8 crate9 crate10 crate11 crate12 crate13 crate14 crate15 crate16 crate17 crate18 crate19 crate20 crate21 crate22 crate23 crate24 crate25 crate26 crate27 crate28 crate29 crate30 crate31 crate32 crate33 crate34 crate35 crate36 crate37 crate38 crate39 crate40 crate41 crate42 crate43 crate44 crate45 crate46 crate47 crate48 crate49 crate50 crate51 crate52 crate53 crate54 crate55 crate56 crate57 crate58 crate59 - Crate
	hoist0 hoist1 hoist2 hoist3 hoist4 hoist5 hoist6 hoist7 hoist8 hoist9 hoist10 hoist11 hoist12 hoist13 hoist14 hoist15 hoist16 hoist17 hoist18 hoist19 hoist20 hoist21 hoist22 hoist23 hoist24 hoist25 hoist26 hoist27 hoist28 hoist29 hoist30 hoist31 hoist32 hoist33 hoist34 hoist35 hoist36 hoist37 hoist38 hoist39 - Hoist)
(:init
	(at pallet0 depot0)
	(clear crate35)
	(at pallet1 depot1)
	(clear crate41)
	(at pallet2 depot2)
	(clear crate42)
	(at pallet3 depot3)
	(clear pallet3)
	(at pallet4 depot4)
	(clear crate18)
	(at pallet5 depot5)
	(clear crate49)
	(at pallet6 depot6)
	(clear crate25)
	(at pallet7 depot7)
	(clear pallet7)
	(at pallet8 depot8)
	(clear crate34)
	(at pallet9 depot9)
	(clear crate47)
	(at pallet10 depot10)
	(clear crate33)
	(at pallet11 depot11)
	(clear crate10)
	(at pallet12 depot12)
	(clear crate52)
	(at pallet13 depot13)
	(clear pallet13)
	(at pallet14 depot14)
	(clear crate45)
	(at pallet15 depot15)
	(clear crate36)
	(at pallet16 depot16)
	(clear crate55)
	(at pallet17 depot17)
	(clear crate59)
	(at pallet18 depot18)
	(clear crate48)
	(at pallet19 depot19)
	(clear pallet19)
	(at pallet20 distributor0)
	(clear crate7)
	(at pallet21 distributor1)
	(clear crate20)
	(at pallet22 distributor2)
	(clear crate19)
	(at pallet23 distributor3)
	(clear pallet23)
	(at pallet24 distributor4)
	(clear pallet24)
	(at pallet25 distributor5)
	(clear crate54)
	(at pallet26 distributor6)
	(clear crate44)
	(at pallet27 distributor7)
	(clear crate57)
	(at pallet28 distributor8)
	(clear crate13)
	(at pallet29 distributor9)
	(clear pallet29)
	(at pallet30 distributor10)
	(clear crate46)
	(at pallet31 distributor11)
	(clear crate22)
	(at pallet32 distributor12)
	(clear crate38)
	(at pallet33 distributor13)
	(clear pallet33)
	(at pallet34 distributor14)
	(clear crate30)
	(at pallet35 distributor15)
	(clear crate53)
	(at pallet36 distributor16)
	(clear crate56)
	(at pallet37 distributor17)
	(clear pallet37)
	(at pallet38 distributor18)
	(clear crate58)
	(at pallet39 distributor19)
	(clear pallet39)
	(at truck0 depot15)
	(at truck1 distributor10)
	(at truck2 depot16)
	(at truck3 depot17)
	(at truck4 distributor2)
	(at truck5 depot14)
	(at truck6 depot8)
	(at truck7 depot9)
	(at truck8 depot2)
	(at truck9 depot12)
	(at truck10 distributor12)
	(at truck11 depot15)
	(at truck12 distributor8)
	(at truck13 distributor10)
	(at truck14 distributor3)
	(at truck15 distributor7)
	(at truck16 depot7)
	(at truck17 distributor3)
	(at hoist0 depot0)
	(available hoist0)
	(at hoist1 depot1)
	(available hoist1)
	(at hoist2 depot2)
	(available hoist2)
	(at hoist3 depot3)
	(available hoist3)
	(at hoist4 depot4)
	(available hoist4)
	(at hoist5 depot5)
	(available hoist5)
	(at hoist6 depot6)
	(available hoist6)
	(at hoist7 depot7)
	(available hoist7)
	(at hoist8 depot8)
	(available hoist8)
	(at hoist9 depot9)
	(available hoist9)
	(at hoist10 depot10)
	(available hoist10)
	(at hoist11 depot11)
	(available hoist11)
	(at hoist12 depot12)
	(available hoist12)
	(at hoist13 depot13)
	(available hoist13)
	(at hoist14 depot14)
	(available hoist14)
	(at hoist15 depot15)
	(available hoist15)
	(at hoist16 depot16)
	(available hoist16)
	(at hoist17 depot17)
	(available hoist17)
	(at hoist18 depot18)
	(available hoist18)
	(at hoist19 depot19)
	(available hoist19)
	(at hoist20 distributor0)
	(available hoist20)
	(at hoist21 distributor1)
	(available hoist21)
	(at hoist22 distributor2)
	(available hoist22)
	(at hoist23 distributor3)
	(available hoist23)
	(at hoist24 distributor4)
	(available hoist24)
	(at hoist25 distributor5)
	(available hoist25)
	(at hoist26 distributor6)
	(available hoist26)
	(at hoist27 distributor7)
	(available hoist27)
	(at hoist28 distributor8)
	(available hoist28)
	(at hoist29 distributor9)
	(available hoist29)
	(at hoist30 distributor10)
	(available hoist30)
	(at hoist31 distributor11)
	(available hoist31)
	(at hoist32 distributor12)
	(available hoist32)
	(at hoist33 distributor13)
	(available hoist33)
	(at hoist34 distributor14)
	(available hoist34)
	(at hoist35 distributor15)
	(available hoist35)
	(at hoist36 distributor16)
	(available hoist36)
	(at hoist37 distributor17)
	(available hoist37)
	(at hoist38 distributor18)
	(available hoist38)
	(at hoist39 distributor19)
	(available hoist39)
	(at crate0 distributor5)
	(on crate0 pallet25)
	(at crate1 depot14)
	(on crate1 pallet14)
	(at crate2 distributor7)
	(on crate2 pallet27)
	(at crate3 depot9)
	(on crate3 pallet9)
	(at crate4 depot6)
	(on crate4 pallet6)
	(at crate5 depot0)
	(on crate5 pallet0)
	(at crate6 distributor10)
	(on crate6 pallet30)
	(at crate7 distributor0)
	(on crate7 pallet20)
	(at crate8 depot18)
	(on crate8 pallet18)
	(at crate9 distributor2)
	(on crate9 pallet22)
	(at crate10 depot11)
	(on crate10 pallet11)
	(at crate11 depot12)
	(on crate11 pallet12)
	(at crate12 depot9)
	(on crate12 crate3)
	(at crate13 distributor8)
	(on crate13 pallet28)
	(at crate14 distributor1)
	(on crate14 pallet21)
	(at crate15 distributor12)
	(on crate15 pallet32)
	(at crate16 distributor14)
	(on crate16 pallet34)
	(at crate17 distributor18)
	(on crate17 pallet38)
	(at crate18 depot4)
	(on crate18 pallet4)
	(at crate19 distributor2)
	(on crate19 crate9)
	(at crate20 distributor1)
	(on crate20 crate14)
	(at crate21 depot5)
	(on crate21 pallet5)
	(at crate22 distributor11)
	(on crate22 pallet31)
	(at crate23 depot5)
	(on crate23 crate21)
	(at crate24 depot18)
	(on crate24 crate8)
	(at crate25 depot6)
	(on crate25 crate4)
	(at crate26 depot10)
	(on crate26 pallet10)
	(at crate27 depot18)
	(on crate27 crate24)
	(at crate28 depot15)
	(on crate28 pallet15)
	(at crate29 distributor5)
	(on crate29 crate0)
	(at crate30 distributor14)
	(on crate30 crate16)
	(at crate31 depot2)
	(on crate31 pallet2)
	(at crate32 distributor5)
	(on crate32 crate29)
	(at crate33 depot10)
	(on crate33 crate26)
	(at crate34 depot8)
	(on crate34 pallet8)
	(at crate35 depot0)
	(on crate35 crate5)
	(at crate36 depot15)
	(on crate36 crate28)
	(at crate37 depot1)
	(on crate37 pallet1)
	(at crate38 distributor12)
	(on crate38 crate15)
	(at crate39 depot17)
	(on crate39 pallet17)
	(at crate40 distributor15)
	(on crate40 pallet35)
	(at crate41 depot1)
	(on crate41 crate37)
	(at crate42 depot2)
	(on crate42 crate31)
	(at crate43 distributor7)
	(on crate43 crate2)
	(at crate44 distributor6)
	(on crate44 pallet26)
	(at crate45 depot14)
	(on crate45 crate1)
	(at crate46 distributor10)
	(on crate46 crate6)
	(at crate47 depot9)
	(on crate47 crate12)
	(at crate48 depot18)
	(on crate48 crate27)
	(at crate49 depot5)
	(on crate49 crate23)
	(at crate50 depot12)
	(on crate50 crate11)
	(at crate51 depot16)
	(on crate51 pallet16)
	(at crate52 depot12)
	(on crate52 crate50)
	(at crate53 distributor15)
	(on crate53 crate40)
	(at crate54 distributor5)
	(on crate54 crate32)
	(at crate55 depot16)
	(on crate55 crate51)
	(at crate56 distributor16)
	(on crate56 pallet36)
	(at crate57 distributor7)
	(on crate57 crate43)
	(at crate58 distributor18)
	(on crate58 crate17)
	(at crate59 depot17)
	(on crate59 crate39)
)

(:goal (and
		(on crate0 pallet19)
		(on crate1 pallet16)
		(on crate2 crate6)
		(on crate3 crate58)
		(on crate5 pallet0)
		(on crate6 crate23)
		(on crate7 crate40)
		(on crate9 pallet20)
		(on crate10 crate19)
		(on crate11 pallet24)
		(on crate12 pallet3)
		(on crate13 pallet34)
		(on crate14 crate15)
		(on crate15 pallet7)
		(on crate16 crate22)
		(on crate17 crate45)
		(on crate18 crate51)
		(on crate19 crate48)
		(on crate20 pallet33)
		(on crate21 pallet12)
		(on crate22 pallet17)
		(on crate23 crate25)
		(on crate24 pallet32)
		(on crate25 pallet9)
		(on crate26 crate5)
		(on crate27 pallet10)
		(on crate28 pallet22)
		(on crate29 crate42)
		(on crate30 pallet25)
		(on crate32 crate37)
		(on crate33 crate56)
		(on crate34 pallet29)
		(on crate36 pallet31)
		(on crate37 pallet26)
		(on crate38 crate0)
		(on crate39 crate34)
		(on crate40 pallet27)
		(on crate41 pallet35)
		(on crate42 crate27)
		(on crate43 crate49)
		(on crate45 crate20)
		(on crate46 pallet36)
		(on crate47 pallet21)
		(on crate48 pallet6)
		(on crate49 crate41)
		(on crate51 pallet38)
		(on crate53 pallet4)
		(on crate54 pallet13)
		(on crate56 pallet14)
		(on crate57 pallet39)
		(on crate58 pallet28)
		(on crate59 pallet15)
	)
))