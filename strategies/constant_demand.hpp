/// \file   constant_demand.hpp
///
/// \brief
///
/// \authors    Maarten P. Scholl
/// \date       2020-04-04
/// \copyright  Copyright 2017-2020 The Institute for New Economic Thinking,
///             Oxford Martin School, University of Oxford
///
///             Licensed under the Apache License, Version 2.0 (the "License");
///             you may not use this file except in compliance with the License.
///             You may obtain a copy of the License at
///
///                 http://www.apache.org/licenses/LICENSE-2.0
///
///             Unless required by applicable law or agreed to in writing,
///             software distributed under the License is distributed on an "AS
///             IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
///             express or implied. See the License for the specific language
///             governing permissions and limitations under the License.
///
///             You may obtain instructions to fulfill the attribution
///             requirements in CITATION.cff
///
#ifndef ME_CONSTANT_DEMAND_HPP
#define ME_CONSTANT_DEMAND_HPP

#include "../fund.hpp"


#include <esl/economics/markets/walras/quote_message.hpp>
#include <esl/economics/markets/walras/differentiable_order_message.hpp>

using namespace esl;
using simulation::time_point;
using simulation::time_interval;
using namespace esl::economics::markets::walras;
using esl::economics::currencies::USD;
using law::jurisdictions::US;

#include <map>


class constant_demand
: public fund
{
public:
    explicit constant_demand(const identity<fund> &i, const jurisdiction &j = law::jurisdictions::US);

    std::map<identity<law::property>, double> demand;

    time_point invest(std::shared_ptr<esl::economics::markets::walras::quote_message> message,
                      time_interval interval, std::seed_seq &seed) override;

    std::string describe() const override;
};



///
/// \brief
///
struct constant_demand_ddsf
        : public differentiable_order_message
{
    esl::economics::price net_asset_value;

    ///
    /// \brief variates[i] contains a factor for quotes[i].
    ///
    /// \param sender
    /// \param recipient
    /// \param sent
    /// \param received
    std::map<identity<law::property>, double> variates;

    constant_demand_ddsf
            ( const identity<agent> &sender
                    , const identity<agent> &recipient

                    , time_point sent     = time_point()
                    , time_point received = time_point()
                    , esl::economics::price nav = price::approximate(0.00, USD)
                    , std::map<identity<law::property>, double> variates = {}

            );


    virtual std::map<identity<law::property>, variable>
    excess_demand(
            const std::map<identity<law::property>,
                    std::tuple<economics::markets::quote, variable>>
            &quotes) const override;

    virtual /*C++20 constexpr*/ std::string name() const
    {
        return "constant demand trader";
    }

    template<class archive_t>
    void serialize(archive_t &archive, const unsigned int version)
    {
        (void)version;
        archive &BOOST_SERIALIZATION_BASE_OBJECT_NVP(differentiable_order_message);
    }
};


#endif  // ME_CONSTANT_DEMAND_HPP
