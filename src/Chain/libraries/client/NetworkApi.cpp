#include <client/Client.hpp>
#include <client/ClientImpl.hpp>
#include <client/Messages.hpp>

namespace thinkyoung {
    namespace client {
        namespace detail {

			// 트랜잭션리스트를 방송하는 함수
            thinkyoung::blockchain::TransactionIdType detail::ClientImpl::network_broadcast_transactions(const std::vector<thinkyoung::blockchain::SignedTransaction>& transactions_to_broadcast)
            {
				TransactionIdType tid;
				std::string strSeedAddr = "";
                // set limit in  sandbox state
                if (_chain_db->get_is_in_sandbox())
                    FC_THROW_EXCEPTION(sandbox_command_forbidden, "in sandbox, this command is forbidden, you cannot call it!");

                // ilog("broadcasting transaction: ${id} ", ("id", transaction_to_broadcast.id()));

                // p2p doesn't send messages back to the originator
				// 기존버전에서는 연결되어 있는 모든 노드들에 다 방송하였다.
				// 새 버전에서는 시드노드가 아닌 경우 시드노드들가운데서 무작위로 선택을 하여
				// 트랜잭션리스트를 전송한다.
				if (!is_seed && !is_status) { //_wallet->is_seed_node() == false) {
					vector<fc::string> seeds = get_active_seed_list();
					//int count = _p2p_node->active_seed_connection_count();
					if (seeds.size() > 0) {
						//int index = rand() % count;
						fc::string strAddr = seeds[rand() % seeds.size()];
						strSeedAddr = _p2p_node->send_to_addr(BatchTrxMessage(transactions_to_broadcast), strAddr);
						//strSeedAddr = _p2p_node->send_to_seed(BatchTrxMessage(transactions_to_broadcast), index);
					}
					if (strSeedAddr != "")
					{ 
						for (auto trx : transactions_to_broadcast) {
							tid = trx.id();
							_chain_db->store_transaction_sending_state(tid, strSeedAddr);
						}
					}
					//_p2p_node->broadcast(BatchTrxMessage(transactions_to_broadcast));
				}
                return transactions_to_broadcast.begin()->id();
            }

			void detail::ClientImpl::network_send_transaction(const thinkyoung::blockchain::SignedTransaction& transaction_to_broadcast, string strSeedAddr)
			{
				std::string strAddr = "";
				TransactionIdType tid;
				// set limit in  sandbox state
				if (_chain_db->get_is_in_sandbox())
					FC_THROW_EXCEPTION(sandbox_command_forbidden, "in sandbox, this command is forbidden, you cannot call it!");

				if ((!is_seed) && (!is_status)) { //_wallet->is_seed_node() == false) {
					strAddr = _p2p_node->send_to_addr(TrxMessage(transaction_to_broadcast), strSeedAddr);

					if (strAddr != "")
					{
						tid = transaction_to_broadcast.id();
						_chain_db->store_transaction_sending_state(tid, strAddr);
					}
				}
			}

			// 트랜잭션를 방송하는 함수
			thinkyoung::blockchain::TransactionIdType detail::ClientImpl::network_broadcast_transaction(const thinkyoung::blockchain::SignedTransaction& transaction_to_broadcast)
            {
				std::string strSeedAddr = "";
				TransactionIdType tid;
                // set limit in  sandbox state
                if (_chain_db->get_is_in_sandbox())
                    FC_THROW_EXCEPTION(sandbox_command_forbidden, "in sandbox, this command is forbidden, you cannot call it!");

				if ((!is_seed) && (!is_status)) { //_wallet->is_seed_node() == false) {
					//int count = _p2p_node->active_seed_connection_count();
					//if (count > 0) {
					//	int index = rand() % count;
					//	strSeedAddr = _p2p_node->send_to_seed(TrxMessage(transaction_to_broadcast), index);
					broadcast_trx_mutex.lock();

					optional<TransactionSendingState> ostate = _chain_db->get_transaction_sending_state(transaction_to_broadcast.id());
					vector<fc::string> seeds = get_active_seed_list();

					if (!ostate.valid()) {
						//int count = _p2p_node->active_seed_connection_count();
						if (seeds.size() > 0) {
							//int index = rand() % count;
							fc::string strAddr = seeds[rand() % seeds.size()];
							strSeedAddr = _p2p_node->send_to_addr(TrxMessage(transaction_to_broadcast), strAddr);

							ilog("CLIENT: send transaction ${id} to ${peer}", ("id", transaction_to_broadcast.id())("peer", strSeedAddr));
						}
					}
					else {
						if (seeds.size() > 0) {
							fc::string strAddr = ostate->seedAddr;
							for (int i = 0; i < seeds.size(); i++) {
								if (seeds[i] == strAddr) {
									strSeedAddr = _p2p_node->send_to_addr(TrxMessage(transaction_to_broadcast), strAddr);
									//ilog("CLIENT: send transaction ${id} to ${peer}", ("id", transaction_to_broadcast.id())("peer", strSeedAddr));

									break;
								}
							}
						}
					}

					if (strSeedAddr != "")
					{
						tid = transaction_to_broadcast.id();
						_chain_db->store_transaction_sending_state(tid, strSeedAddr);
					}

					broadcast_trx_mutex.unlock();
				}
                return transaction_to_broadcast.id();
            }

            std::vector<fc::variant_object> detail::ClientImpl::network_get_peer_info(bool hide_firewalled_nodes)const
            {
                std::vector<fc::variant_object> results;
                std::vector<thinkyoung::net::PeerStatus> peer_statuses = _p2p_node->get_connected_peers();
                for (const thinkyoung::net::PeerStatus& peer_status : peer_statuses)
                    if (!hide_firewalled_nodes ||
                        peer_status.info["firewall_status"].as_string() == "not_firewalled")
                        results.push_back(peer_status.info);
                return results;
            }

            std::vector<fc::variant_object> detail::ClientImpl::status_nodes_info(bool hide_firewalled_nodes) const
            {
                std::vector<fc::variant_object> results;
				get_active_status_node_count();
				if (is_status) {
					fc::mutable_variant_object i;
					i["index"] = status_index;
					i["is_master"] = is_master_status_node();
					i["total_wallets_unlocked"] = status_node_unlocked_wallets_count();
					i["active_time"] = status_active_time;
					results.push_back(i);
				}
				for (auto info : status_node_list) {
					fc::mutable_variant_object i;
					i["index"] = info.index;
					i["is_master"] = is_master_status_node(info.index);
					if(is_status) i["total_wallets_unlocked"] = status_node_unlocked_wallets_count();
					i["active_time"] = info.active_time;
					results.push_back(i);
				}
                return results;
            }

            std::vector<fc::variant_object> detail::ClientImpl::status_node_unlocked_wallets_list(bool hide_firewalled_nodes) const
            {
				if(!is_status) FC_THROW_EXCEPTION(cant_exec_on_non_status, "This command can execute only on status nodes.");

                std::vector<fc::variant_object> results;
				int index = 0;
				for (auto w : wallet_status_list) {
					fc::mutable_variant_object i;
					i["index"] = index++;
					i["wallet_name"] = w.wallet_name;
					i["unlock_at"] = w.unlocked_time;
					i["unlock_until"] = w.expire_time;
					results.push_back(i);
				}
                return results;
            }

            // void detail::client_impl::network_set_allowed_peers(const vector<thinkyoung::net::node_id_t>& allowed_peers)
            // {
            //    _p2p_node->set_allowed_peers( allowed_peers );
            // }

            void detail::ClientImpl::network_set_advanced_node_parameters(const fc::variant_object& params)
            {
                // set limit in  sandbox state
                if (_chain_db->get_is_in_sandbox())
                    FC_THROW_EXCEPTION(sandbox_command_forbidden, "in sandbox, this command is forbidden, you cannot call it!");

                _p2p_node->set_advanced_node_parameters(params);
            }

            fc::variant_object detail::ClientImpl::network_get_advanced_node_parameters() const
            {
                return _p2p_node->get_advanced_node_parameters();
            }

            void detail::ClientImpl::network_add_node(const string& node, const string& command)
            {
                if (command == "add")
                    _self->connect_to_peer(node);
                else if (command == "block")
                {
                    std::basic_string <char> ip;
                    size_t pos;
                    if ((pos = node.find(':')) != std::string::npos)
                    {
                        _p2p_node->block_node(node.substr(0, pos).c_str(), true);
                        _p2p_node->add_node(fc::ip::endpoint::from_string(node), -1);
                    }
                    else
                    {
                        _p2p_node->block_node(node, true);
                        _p2p_node->add_node(fc::ip::endpoint::from_string(node + ":0"), -1);
                    }
                }
                else if (command == "unblock")
                {
                    std::basic_string <char> ip;
                    size_t pos;
                    if ((pos = node.find(':')) != std::string::npos)
                        _p2p_node->block_node(node.substr(0, pos).c_str(), false);
                    else
                        _p2p_node->block_node(node, false);
                }
                else
                {
                    FC_THROW_EXCEPTION(fc::invalid_arg_exception, "unsupported command argument \"${command}\", valid commands are: \"add\"", ("command", command));
                }
            }

            uint32_t detail::ClientImpl::network_get_connection_count() const
            {
                return _p2p_node->get_connection_count();
            }

            thinkyoung::net::MessagePropagationData detail::ClientImpl::network_get_transaction_propagation_data(const TransactionIdType& transaction_id)
            {
                // set limit in  sandbox state
                if (_chain_db->get_is_in_sandbox())
                    FC_THROW_EXCEPTION(sandbox_command_forbidden, "in sandbox, this command is forbidden, you cannot call it!");

                return _p2p_node->get_transaction_propagation_data(transaction_id);
                FC_THROW_EXCEPTION(fc::invalid_operation_exception, "get_transaction_propagation_data only valid in p2p mode");
            }

            thinkyoung::net::MessagePropagationData detail::ClientImpl::network_get_block_propagation_data(const BlockIdType& block_id)
            {
                // set limit in  sandbox state
                if (_chain_db->get_is_in_sandbox())
                    FC_THROW_EXCEPTION(sandbox_command_forbidden, "in sandbox, this command is forbidden, you cannot call it!");

                return _p2p_node->get_block_propagation_data(block_id);
                FC_THROW_EXCEPTION(fc::invalid_operation_exception, "get_block_propagation_data only valid in p2p mode");
            }

            fc::variant_object ClientImpl::network_get_info() const
            {
                return _p2p_node->network_get_info();
            }

            // fc::variant_object client_impl::network_get_usage_stats() const
            // {
            //    return _p2p_node->network_get_usage_stats();
            // }

            vector<thinkyoung::net::PotentialPeerEntry> ClientImpl::network_list_potential_peers()const
            {
                return _p2p_node->get_potential_peers();
            }

            fc::variant_object ClientImpl::network_get_upnp_info()const
            {
                fc::mutable_variant_object upnp_info;

                upnp_info["upnp_enabled"] = bool(_upnp_service);

                if (_upnp_service)
                {
                    upnp_info["external_ip"] = fc::string(_upnp_service->external_ip());
                    upnp_info["mapped_port"] = fc::variant(_upnp_service->mapped_port()).as_string();
                }

                return upnp_info;
            }

            std::vector<std::string> ClientImpl::network_get_blocked_ips()const
            {
                return _p2p_node->get_blocked_ips();
            }

        }
    }
} // namespace thinkyoung::client::detail
