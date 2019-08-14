(define (problem depotprob19) (:domain depots)
(:objects
	depot0 depot1 depot2 depot3 depot4 depot5 depot6 depot7 depot8 depot9 depot10 depot11 depot12 depot13 depot14 depot15 depot16 depot17 depot18 depot19 depot20 depot21 - Depot
	distributor0 distributor1 distributor2 distributor3 distributor4 distributor5 distributor6 distributor7 distributor8 distributor9 distributor10 distributor11 distributor12 distributor13 distributor14 distributor15 distributor16 distributor17 distributor18 distributor19 distributor20 distributor21 - Distributor
	truck0 truck1 truck2 truck3 truck4 truck5 truck6 truck7 truck8 truck9 truck10 truck11 truck12 truck13 truck14 truck15 truck16 truck17 truck18 truck19 - Truck
	pallet0 pallet1 pallet2 pallet3 pallet4 pallet5 pallet6 pallet7 pallet8 pallet9 pallet10 pallet11 pallet12 pallet13 pallet14 pallet15 pallet16 pallet17 pallet18 pallet19 pallet20 pallet21 pallet22 pallet23 pallet24 pallet25 pallet26 pallet27 pallet28 pallet29 pallet30 pallet31 pallet32 pallet33 pallet34 pallet35 pallet36 pallet37 pallet38 pallet39 pallet40 pallet41 pallet42 pallet43 - Pallet
	crate0 crate1 crate2 crate3 crate4 crate5 crate6 crate7 crate8 crate9 crate10 crate11 crate12 crate13 crate14 crate15 crate16 crate17 crate18 crate19 crate20 crate21 crate22 crate23 crate24 crate25 crate26 crate27 crate28 crate29 crate30 crate31 crate32 crate33 crate34 crate35 crate36 crate37 crate38 crate39 crate40 crate41 crate42 crate43 crate44 crate45 crate46 crate47 crate48 crate49 crate50 crate51 crate52 crate53 crate54 crate55 crate56 crate57 crate58 crate59 crate60 crate61 crate62 crate63 crate64 crate65 - Crate
	hoist0 hoist1 hoist2 hoist3 hoist4 hoist5 hoist6 hoist7 hoist8 hoist9 hoist10 hoist11 hoist12 hoist13 hoist14 hoist15 hoist16 hoist17 hoist18 hoist19 hoist20 hoist21 hoist22 hoist23 hoist24 hoist25 hoist26 hoist27 hoist28 hoist29 hoist30 hoist31 hoist32 hoist33 hoist34 hoist35 hoist36 hoist37 hoist38 hoist39 hoist40 hoist41 hoist42 hoist43 - Hoist)
(:init
	(at pallet0 depot0)
	(clear crate47)
	(at pallet1 depot1)
	(clear pallet1)
	(at pallet2 depot2)
	(clear crate10)
	(at pallet3 depot3)
	(clear pallet3)
	(at pallet4 depot4)
	(clear pallet4)
	(at pallet5 depot5)
	(clear crate22)
	(at pallet6 depot6)
	(clear crate60)
	(at pallet7 depot7)
	(clear crate51)
	(at pallet8 depot8)
	(clear crate64)
	(at pallet9 depot9)
	(clear crate2)
	(at pallet10 depot10)
	(clear crate49)
	(at pallet11 depot11)
	(clear crate54)
	(at pallet12 depot12)
	(clear crate50)
	(at pallet13 depot13)
	(clear crate44)
	(at pallet14 depot14)
	(clear crate53)
	(at pallet15 depot15)
	(clear crate34)
	(at pallet16 depot16)
	(clear crate7)
	(at pallet17 depot17)
	(clear pallet17)
	(at pallet18 depot18)
	(clear crate40)
	(at pallet19 depot19)
	(clear crate6)
	(at pallet20 depot20)
	(clear crate36)
	(at pallet21 depot21)
	(clear crate58)
	(at pallet22 distributor0)
	(clear crate27)
	(at pallet23 distributor1)
	(clear crate46)
	(at pallet24 distributor2)
	(clear crate20)
	(at pallet25 distributor3)
	(clear crate45)
	(at pallet26 distributor4)
	(clear crate41)
	(at pallet27 distributor5)
	(clear crate29)
	(at pallet28 distributor6)
	(clear pallet28)
	(at pallet29 distributor7)
	(clear crate19)
	(at pallet30 distributor8)
	(clear pallet30)
	(at pallet31 distributor9)
	(clear crate13)
	(at pallet32 distributor10)
	(clear crate26)
	(at pallet33 distributor11)
	(clear crate35)
	(at pallet34 distributor12)
	(clear crate62)
	(at pallet35 distributor13)
	(clear crate17)
	(at pallet36 distributor14)
	(clear pallet36)
	(at pallet37 distributor15)
	(clear pallet37)
	(at pallet38 distributor16)
	(clear crate61)
	(at pallet39 distributor17)
	(clear crate48)
	(at pallet40 distributor18)
	(clear crate9)
	(at pallet41 distributor19)
	(clear crate65)
	(at pallet42 distributor20)
	(clear pallet42)
	(at pallet43 distributor21)
	(clear crate32)
	(at truck0 depot20)
	(at truck1 depot3)
	(at truck2 depot12)
	(at truck3 distributor14)
	(at truck4 distributor14)
	(at truck5 distributor18)
	(at truck6 depot16)
	(at truck7 distributor6)
	(at truck8 depot4)
	(at truck9 depot18)
	(at truck10 distributor11)
	(at truck11 depot18)
	(at truck12 distributor0)
	(at truck13 distributor9)
	(at truck14 depot20)
	(at truck15 distributor16)
	(at truck16 depot5)
	(at truck17 distributor10)
	(at truck18 distributor16)
	(at truck19 depot8)
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
	(at hoist20 depot20)
	(available hoist20)
	(at hoist21 depot21)
	(available hoist21)
	(at hoist22 distributor0)
	(available hoist22)
	(at hoist23 distributor1)
	(available hoist23)
	(at hoist24 distributor2)
	(available hoist24)
	(at hoist25 distributor3)
	(available hoist25)
	(at hoist26 distributor4)
	(available hoist26)
	(at hoist27 distributor5)
	(available hoist27)
	(at hoist28 distributor6)
	(available hoist28)
	(at hoist29 distributor7)
	(available hoist29)
	(at hoist30 distributor8)
	(available hoist30)
	(at hoist31 distributor9)
	(available hoist31)
	(at hoist32 distributor10)
	(available hoist32)
	(at hoist33 distributor11)
	(available hoist33)
	(at hoist34 distributor12)
	(available hoist34)
	(at hoist35 distributor13)
	(available hoist35)
	(at hoist36 distributor14)
	(available hoist36)
	(at hoist37 distributor15)
	(available hoist37)
	(at hoist38 distributor16)
	(available hoist38)
	(at hoist39 distributor17)
	(available hoist39)
	(at hoist40 distributor18)
	(available hoist40)
	(at hoist41 distributor19)
	(available hoist41)
	(at hoist42 distributor20)
	(available hoist42)
	(at hoist43 distributor21)
	(available hoist43)
	(at crate0 distributor19)
	(on crate0 pallet41)
	(at crate1 depot20)
	(on crate1 pallet20)
	(at crate2 depot9)
	(on crate2 pallet9)
	(at crate3 distributor21)
	(on crate3 pallet43)
	(at crate4 depot11)
	(on crate4 pallet11)
	(at crate5 distributor13)
	(on crate5 pallet35)
	(at crate6 depot19)
	(on crate6 pallet19)
	(at crate7 depot16)
	(on crate7 pallet16)
	(at crate8 distributor4)
	(on crate8 pallet26)
	(at crate9 distributor18)
	(on crate9 pallet40)
	(at crate10 depot2)
	(on crate10 pallet2)
	(at crate11 depot15)
	(on crate11 pallet15)
	(at crate12 depot18)
	(on crate12 pallet18)
	(at crate13 distributor9)
	(on crate13 pallet31)
	(at crate14 distributor11)
	(on crate14 pallet33)
	(at crate15 distributor19)
	(on crate15 crate0)
	(at crate16 depot6)
	(on crate16 pallet6)
	(at crate17 distributor13)
	(on crate17 crate5)
	(at crate18 distributor11)
	(on crate18 crate14)
	(at crate19 distributor7)
	(on crate19 pallet29)
	(at crate20 distributor2)
	(on crate20 pallet24)
	(at crate21 distributor1)
	(on crate21 pallet23)
	(at crate22 depot5)
	(on crate22 pallet5)
	(at crate23 depot13)
	(on crate23 pallet13)
	(at crate24 depot7)
	(on crate24 pallet7)
	(at crate25 depot11)
	(on crate25 crate4)
	(at crate26 distributor10)
	(on crate26 pallet32)
	(at crate27 distributor0)
	(on crate27 pallet22)
	(at crate28 depot14)
	(on crate28 pallet14)
	(at crate29 distributor5)
	(on crate29 pallet27)
	(at crate30 distributor11)
	(on crate30 crate18)
	(at crate31 distributor11)
	(on crate31 crate30)
	(at crate32 distributor21)
	(on crate32 crate3)
	(at crate33 depot13)
	(on crate33 crate23)
	(at crate34 depot15)
	(on crate34 crate11)
	(at crate35 distributor11)
	(on crate35 crate31)
	(at crate36 depot20)
	(on crate36 crate1)
	(at crate37 distributor17)
	(on crate37 pallet39)
	(at crate38 depot7)
	(on crate38 crate24)
	(at crate39 depot13)
	(on crate39 crate33)
	(at crate40 depot18)
	(on crate40 crate12)
	(at crate41 distributor4)
	(on crate41 crate8)
	(at crate42 depot8)
	(on crate42 pallet8)
	(at crate43 depot14)
	(on crate43 crate28)
	(at crate44 depot13)
	(on crate44 crate39)
	(at crate45 distributor3)
	(on crate45 pallet25)
	(at crate46 distributor1)
	(on crate46 crate21)
	(at crate47 depot0)
	(on crate47 pallet0)
	(at crate48 distributor17)
	(on crate48 crate37)
	(at crate49 depot10)
	(on crate49 pallet10)
	(at crate50 depot12)
	(on crate50 pallet12)
	(at crate51 depot7)
	(on crate51 crate38)
	(at crate52 depot11)
	(on crate52 crate25)
	(at crate53 depot14)
	(on crate53 crate43)
	(at crate54 depot11)
	(on crate54 crate52)
	(at crate55 distributor12)
	(on crate55 pallet34)
	(at crate56 depot8)
	(on crate56 crate42)
	(at crate57 distributor16)
	(on crate57 pallet38)
	(at crate58 depot21)
	(on crate58 pallet21)
	(at crate59 distributor16)
	(on crate59 crate57)
	(at crate60 depot6)
	(on crate60 crate16)
	(at crate61 distributor16)
	(on crate61 crate59)
	(at crate62 distributor12)
	(on crate62 crate55)
	(at crate63 depot8)
	(on crate63 crate56)
	(at crate64 depot8)
	(on crate64 crate63)
	(at crate65 distributor19)
	(on crate65 crate15)
)

(:goal (and
		(on crate0 pallet33)
		(on crate1 pallet37)
		(on crate3 pallet38)
		(on crate5 pallet19)
		(on crate6 pallet9)
		(on crate7 pallet31)
		(on crate8 crate9)
		(on crate9 pallet30)
		(on crate10 pallet4)
		(on crate11 pallet39)
		(on crate12 pallet8)
		(on crate13 pallet36)
		(on crate15 crate19)
		(on crate17 crate29)
		(on crate18 crate48)
		(on crate19 pallet40)
		(on crate20 pallet11)
		(on crate21 pallet10)
		(on crate22 crate27)
		(on crate23 pallet15)
		(on crate24 pallet12)
		(on crate25 crate32)
		(on crate26 crate62)
		(on crate27 crate11)
		(on crate28 crate44)
		(on crate29 pallet16)
		(on crate30 pallet18)
		(on crate31 pallet42)
		(on crate32 pallet6)
		(on crate33 pallet34)
		(on crate34 pallet28)
		(on crate35 pallet5)
		(on crate36 pallet22)
		(on crate38 crate28)
		(on crate39 crate59)
		(on crate40 crate61)
		(on crate41 pallet25)
		(on crate42 pallet23)
		(on crate43 crate31)
		(on crate44 pallet7)
		(on crate45 crate41)
		(on crate46 pallet20)
		(on crate47 crate3)
		(on crate48 crate47)
		(on crate49 crate0)
		(on crate50 pallet32)
		(on crate51 crate6)
		(on crate52 pallet13)
		(on crate53 crate42)
		(on crate54 crate12)
		(on crate55 crate38)
		(on crate56 pallet21)
		(on crate57 pallet27)
		(on crate58 crate43)
		(on crate59 crate35)
		(on crate60 crate21)
		(on crate61 crate17)
		(on crate62 pallet26)
		(on crate63 crate53)
		(on crate64 crate1)
		(on crate65 crate52)
	)
))
